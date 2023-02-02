#pragma once
#include "text_renderer.h"

// A class for text captions, which takes intended center-top coordinates and calculates own position
// based on given scale and textstring size.
class TextCaption
{
public:
	TextCaption(std::string text, float centerX, float y, float scale,
		std::shared_ptr <TextRenderer> textrenderer, glm::vec3 color = glm::vec3(1.0f));
	void DrawText();
	void SetCaption(std::string newtext);
private:
	std::string TextString;
	float positionX, positionY, Scale;
	std::shared_ptr <TextRenderer> OwnTextRenderer;
	glm::vec3 Color;
};