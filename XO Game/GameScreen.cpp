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
			if(gameobject->isVisible)
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
			if(interactiveelement->GetActiveState()
				&& interactiveelement->isMouseOnInteractiveObject(MousePosition))
				interactiveelement->onMouseClick(MousePosition);
		}
	}
}

void GameScreen::SetActiveState(bool newstate)
{
	for (IMouseInteractive* interactiveelement : InteractiveElements)
	{
		interactiveelement->SetActiveState(newstate);
	}
	for (GameObject* gameobject : DrawableElements)
	{
		gameobject->isVisible = newstate; // TODO: remove if not needed
	}
}