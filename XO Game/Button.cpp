#include "Button.h"

Button::Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite) :
	GameObject(pos, size, sprite),
	IMouseInteractive(pos, size), isPressed(false) { }


void Button::onMouseClick(glm::vec2 MousePosition)
{
	this->isPressed = true;
}