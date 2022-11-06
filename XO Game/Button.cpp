#include "button.h"

Button::Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite) :
	GameObject(pos, size, sprite),
	IMouseInteractive(pos, size), isPressed(false), ButtonPressedSprite(sprite) { }

Button::Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite, Texture2D buttonpressedsprite) :
	GameObject(pos, size, sprite),
	IMouseInteractive(pos, size), isPressed(false), ButtonPressedSprite(buttonpressedsprite) { }

void Button::onMouseClick(glm::vec2 MousePosition)
{
	this->isPressed = true;
}

void Button::Draw(SpriteRenderer& renderer)
{
	if(!isPressed)
		renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	else
		renderer.DrawSprite(this->ButtonPressedSprite, this->Position, this->Size, this->Rotation, this->Color);
}