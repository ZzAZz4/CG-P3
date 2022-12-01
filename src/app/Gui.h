

#ifndef OGL_GUI_H
#define OGL_GUI_H

#include "../rendering/GLWindow.h"

class Gui {
public:
    Gui(const GLWindow& window);
    virtual ~Gui();

    void Render();

    void CreateFrame();
    void EndFrame();

    virtual void SetupFrame() = 0;
};

#endif // OGL_GUI_H
