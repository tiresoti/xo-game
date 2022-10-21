#pragma once

#include "glad/include/glad/glad.h"

#include "framework/texture.h"
#include "framework/sprite_renderer.h"


// Container object for holding all state relevant for a single
// drawable game object entity.
class GameObject
{
public:
    // object state
    glm::vec2   Position, Size;
    glm::vec3   Color;
    float       Rotation;
    bool        Destroyed;//TODO: убрать или задефолтить
    // render state
    Texture2D   Sprite;	
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);
    void SetTexture(Texture2D newsprite); //TODO мб убрать в клетку
};
