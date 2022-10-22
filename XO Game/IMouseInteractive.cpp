#include "IMouseInteractive.h"

IMouseInteractive::IMouseInteractive() { }

IMouseInteractive::IMouseInteractive(glm::vec2 pos, glm::vec2 size):
	InteractivePosition(pos), InteractiveSize(size) { }

void IMouseInteractive::onMouseClick(glm::vec2 MousePosition) { }

bool IMouseInteractive::isMouseOnInteractiveObject(glm::vec2 MousePosition)
{
	return (MousePosition.x >= InteractivePosition.x
		&& MousePosition.x <= InteractivePosition.x + InteractiveSize.x
		&& MousePosition.y >= InteractivePosition.y
		&& MousePosition.y <= InteractivePosition.y + InteractiveSize.y
		);
}