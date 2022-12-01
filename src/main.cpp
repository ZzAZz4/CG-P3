#include <game/MeshRenderer.h>

int main () {
    MeshRenderer::GenerateMeshes("../data", "../res/animation");
    MeshRenderer app;
    app.Run();
    return 0;
}
