#include "Button.h"

Button::Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite) :
	GameObject(pos, size, sprite), isActive(true), isPressed(false) { }

void Button::SetActiveState(bool activestate)
{
	isActive = activestate;
}
bool Button::GetActiveState()
{
	return isActive;
}
