#ifndef OGL_PLAYER_H
#define OGL_PLAYER_H

#include <app/Camera.h>

struct ActionFlags {
    bool forward : 1 = false;
    bool backward : 1 = false;
    bool left : 1 = false;
    bool right : 1 = false;
    bool up : 1 = false;
    bool down : 1 = false;
    bool run : 1 = false;
};


struct Player {
    Camera camera;
    ActionFlags actionFlags;

    float speed = 2.f; // 0 - 10
    float runSpeed = 4.0f; // 0 - 20
    float mouseSensitivity = 0.1f; // 0 - 1
    bool reclip = false;

    Player();

    void Start();
    void Update();
    void Render();

    void OnMouseMove(double xpos, double ypos);
    void OnKeyPressed(int key, int scancode, int action, int mods);
    void OnResize(int width, int height);
    void OnMouseScroll(double xoffset, double yoffset);
    void OnMouseClick(int button, int action, int mods);
};

#endif // OGL_PLAYER_H
