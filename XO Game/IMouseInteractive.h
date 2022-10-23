#pragma once
#include "glm/glm.hpp"

class IMouseInteractive
{
public:
    IMouseInteractive();
    IMouseInteractive(glm::vec2 pos, glm::vec2 size);
    glm::vec2 InteractivePosition;
    glm::vec2 InteractiveSize;
    virtual void onMouseClick(glm::vec2 MousePosition);
    bool isMouseOnInteractiveObject(glm::vec2 MousePosition);
    void SetActiveState(bool activestate);
    bool GetActiveState();
private:
    bool isActive;
};

