#include "game_screen.h"

GameScreen::GameScreen() { }

void GameScreen::AddDrawable(std::shared_ptr <GameObject> Gameobject)
{
	DrawableElements.push_back(Gameobject);
}

void GameScreen::AddInteractive(std::shared_ptr <IMouseInteractive> InteractiveElement)
{
	InteractiveElements.push_back(InteractiveElement);
}

void GameScreen::AddCaption(std::shared_ptr <TextCaption> TextCaptionElement)
{
	TextElements.push_back(TextCaptionElement);
}

void GameScreen::Draw(std::shared_ptr <SpriteRenderer> Renderer)
{
	if (DrawableElements.size() > 0)
	{
		for (auto gameobject : DrawableElements)
		{
			gameobject->Draw(*Renderer);
		}
	}

	if (TextElements.size() > 0)
	{
		for (auto textcaption : TextElements)
			textcaption->DrawText();
	}

}
void GameScreen::HandleInput(glm::vec2 MousePosition)
{
	if (InteractiveElements.size() > 0)
	{
		for (auto interactiveelement : InteractiveElements)
		{
			if(interactiveelement->GetActiveState()
				&& interactiveelement->isMouseOnInteractiveObject(MousePosition))
				interactiveelement->onMouseClick(MousePosition);
		}
	}
}

void GameScreen::SetActiveState(bool newstate)
{
	for (auto interactiveelement : InteractiveElements)
	{
		interactiveelement->SetActiveState(newstate);
	}
}