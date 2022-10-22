#include "GameScreen.h"

GameScreen::GameScreen() { }

void GameScreen::AddDrawable(GameObject* Gameobject)
{
	DrawableElements.push_back(Gameobject);
}
void GameScreen::AddInteractive(IMouseInteractive* InteractiveElement)
{
	InteractiveElements.push_back(InteractiveElement);
}
void GameScreen::Draw(SpriteRenderer* Renderer)
{
	if (DrawableElements.size() > 0)
	{
		for (GameObject* gameobject : DrawableElements)
		{
			gameobject->Draw(*Renderer);
		}
	}

}
void GameScreen::HandleInput(glm::vec2 MousePosition)
{
	if (InteractiveElements.size() > 0)
	{
		for (IMouseInteractive* interactiveelement : InteractiveElements)
		{
			interactiveelement->onMouseClick(MousePosition);
		}
	}

}
void GameScreen::SetActive()
{
	for (GameObject* gameobject : DrawableElements)
	{
		gameobject->isVisible = true; // TODO: remove if not needed
	}
}
void GameScreen::SetInactive()
{
	for (GameObject* gameobject : DrawableElements)
	{
		gameobject->isVisible = false; // TODO: remove if not needed
	}
}
