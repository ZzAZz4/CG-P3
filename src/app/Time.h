

#ifndef OGL_TIME_H
#define OGL_TIME_H

struct Time {
    using Stamp = float;
    using Delta = float;
    inline static Delta deltaTime;
    inline static Stamp time;

    static void Init();
    static void Update();
    static float GetFPS();
};

#endif // OGL_TIME_H
