#pragma once

#include "game_object.h"
#include "cell_object.h"

class Checkboard : public GameObject
{
public:
	Checkboard(glm::vec2 pos, glm::vec2 size, Texture2D sprite); // init Cells[] here!
	~Checkboard(); // delete Cells[] here!
	Cell* Cells;
	void onMouseClick(int xScreenPos, int yScreenPos);
private:
	Texture2D* EmptyTexture;
	Texture2D* XCrossTexture;
	Texture2D* ORingTexture;
};