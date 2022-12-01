

#ifndef HOMEWORK_MENU_H
#define HOMEWORK_MENU_H

#include <app/Gui.h>

struct MeshRenderer;

class Menu : public Gui {
public:
    explicit Menu(MeshRenderer& app);
    ~Menu() override = default;

    void SetupFrame() override;
private:
    MeshRenderer& app;
};


#endif //HOMEWORK_MENU_H
