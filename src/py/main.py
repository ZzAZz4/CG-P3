import os
import torch
import sys
from pytorch3d.utils import ico_sphere
import numpy as np
from tqdm import tqdm

from pytorch3d.io import load_objs_as_meshes, save_obj

from pytorch3d.loss import (
    mesh_edge_loss,
    mesh_laplacian_smoothing,
    mesh_normal_consistency,
)

from pytorch3d.structures import Meshes
from pytorch3d.renderer import (
    look_at_view_transform,
    FoVPerspectiveCameras,
    PointLights,
    RasterizationSettings,
    MeshRenderer,
    MeshRasterizer,
    SoftPhongShader,
    SoftSilhouetteShader,
    SoftPhongShader,
)


def get_device():
    if torch.cuda.is_available():
        device = torch.device("cuda:0")
        torch.cuda.set_device(device)
    else:
        device = torch.device("cpu")
    return device


def update_mesh_shape_prior_losses(mesh, loss):
    loss["edge"] = mesh_edge_loss(mesh)
    loss["normal"] = mesh_normal_consistency(mesh)
    loss["laplacian"] = mesh_laplacian_smoothing(mesh, method="uniform")


def main():
    argv = sys.argv
    if len(argv) != 3:
        print("Usage: python main.py <data_dir> <path_to_animation>")
        sys.exit(1)

    data_dir = argv[1]
    animation_path = argv[2]

    device = get_device()
    obj_filename = os.path.join(data_dir, "cow_mesh/cow.obj")

    mesh: Meshes = load_objs_as_meshes([obj_filename], device=device)

    verts = mesh.verts_packed()
    center = verts.mean(0)
    scale = max((verts - center).abs().max(0)[0])
    mesh.offset_verts_(-center)
    mesh.scale_verts_((1.0 / float(scale)))

    num_views = 20

    elev = torch.linspace(0, 360, num_views)
    azim = torch.linspace(-180, 180, num_views)

    lights = PointLights(device=device, location=[[0.0, 0.0, -3.0]])

    R, T = look_at_view_transform(dist=2.7, elev=elev, azim=azim)
    cameras = FoVPerspectiveCameras(device=device, R=R, T=T)

    camera = FoVPerspectiveCameras(device=device, R=R[None, 1, ...], T=T[None, 1, ...])

    meshes = mesh.extend(num_views)

    target_cameras = [FoVPerspectiveCameras(device=device, R=R[None, i, ...],
                                            T=T[None, i, ...]) for i in range(num_views)]

    sigma = 1e-4
    raster_settings_silhouette = RasterizationSettings(
        image_size=128,
        blur_radius=np.log(1. / 1e-4 - 1.) * sigma,
        faces_per_pixel=50,
    )

    renderer_silhouette = MeshRenderer(
        rasterizer=MeshRasterizer(
            cameras=camera,
            raster_settings=raster_settings_silhouette
        ),
        shader=SoftSilhouetteShader()
    )

    silhouette_images = renderer_silhouette(meshes, cameras=cameras, lights=lights)
    target_silhouette = [silhouette_images[i, ..., 3] for i in range(num_views)]

    src_mesh = ico_sphere(4, device)

    sigma = 1e-4
    raster_settings_soft = RasterizationSettings(
        image_size=128,
        blur_radius=np.log(1. / 1e-4 - 1.) * sigma,
        faces_per_pixel=50,
    )

    renderer_silhouette = MeshRenderer(
        rasterizer=MeshRasterizer(
            cameras=camera,
            raster_settings=raster_settings_soft
        ),
        shader=SoftSilhouetteShader()
    )

    num_views_per_iteration = 10
    Niter = 1000
    plot_period = 10

    losses = {"silhouette": {"weight": 1.0, "values": []},
              "edge": {"weight": 1.0, "values": []},
              "normal": {"weight": 0.01, "values": []},
              "laplacian": {"weight": 1.0, "values": []},
              }

    verts_shape = src_mesh.verts_packed().shape
    deform_verts = torch.full(verts_shape, 0.0, device=device, requires_grad=True)

    optimizer = torch.optim.Adam([deform_verts], lr=1e-2)

    loop = tqdm(range(Niter + 1))
    os.makedirs(animation_path, exist_ok=True)

    for iteration in loop:
        if iteration == round(Niter * 0.75):
            print('Decreasing LR 10-fold ...')
            optimizer = torch.optim.Adam([deform_verts], lr=1e-3)

        optimizer.zero_grad()
        new_src_mesh = src_mesh.offset_verts(deform_verts)

        loss = {k: torch.tensor(0.0, device=device) for k in losses}
        update_mesh_shape_prior_losses(new_src_mesh, loss)

        selected_randoms = np.random.permutation(num_views).tolist()[:num_views_per_iteration]
        for j in selected_randoms:
            images_predicted = renderer_silhouette(new_src_mesh, cameras=target_cameras[j], lights=lights)
            predicted_silhouette = images_predicted[..., 3]
            loss_silhouette = ((predicted_silhouette - target_silhouette[j]) ** 2).mean()
            loss["silhouette"] += loss_silhouette / num_views_per_iteration

        sum_loss = torch.tensor(0.0, device=device)
        for k, l in loss.items():
            sum_loss += l * losses[k]["weight"]
            losses[k]["values"].append(float(l.detach().cpu()))

            # Print the losses
        loop.set_description("total_loss = %.6f" % sum_loss)
        if iteration % plot_period == 0:
            final_verts, final_faces = new_src_mesh.get_mesh_verts_faces(0)
            final_verts = final_verts * scale + center
            file = os.path.join(animation_path, "model-%04d.obj" % iteration)
            save_obj(file, verts=final_verts, faces=final_faces)

        # Optimization step
        sum_loss.backward()
        optimizer.step()


if __name__ == "__main__":
    main()
