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

void GameScreen::AddCaption(TextCaption* TextCaptionElement)
{
	TextElements.push_back(TextCaptionElement);
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

	if (TextElements.size() > 0)
	{
		for (TextCaption* textcaption : TextElements)
			textcaption->DrawText();
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
}