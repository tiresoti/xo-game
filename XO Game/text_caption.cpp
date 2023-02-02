#include "text_caption.h"

TextCaption::TextCaption(std::string text, float centerX, float y, float scale,
	std::shared_ptr <TextRenderer> textrenderer, glm::vec3 color) :
	TextString(text), Scale(scale), positionX(centerX), positionY(y),
	OwnTextRenderer(textrenderer), Color(color) { }

void TextCaption::DrawText()
{
	OwnTextRenderer->RenderText(this->TextString, this->positionX, this->positionY, this->Scale, this->Color);
}
void TextCaption::SetCaption(std::string newtext)
{
	TextString = std::move(newtext);
}