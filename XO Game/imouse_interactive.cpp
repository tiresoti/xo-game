#include "imouse_interactive.h"

IMouseInteractive::IMouseInteractive() :
	InteractivePosition(glm::vec2(0.0f, 0.0f)), InteractiveSize(glm::vec2(0.0f, 0.0f)), isActive(false) { }

IMouseInteractive::IMouseInteractive(glm::vec2 pos, glm::vec2 size):
	InteractivePosition(pos), InteractiveSize(size), isActive(false) { }

void IMouseInteractive::onMouseClick(glm::vec2 MousePosition) { }

void IMouseInteractive::SetActiveState(bool activestate)
{
	isActive = activestate;
}

bool IMouseInteractive::GetActiveState()
{
	return isActive;
}

bool IMouseInteractive::isMouseOnInteractiveObject(glm::vec2 MousePosition)
{
	return (MousePosition.x >= InteractivePosition.x
		&& MousePosition.x <= InteractivePosition.x + InteractiveSize.x
		&& MousePosition.y >= InteractivePosition.y
		&& MousePosition.y <= InteractivePosition.y + InteractiveSize.y
		);
}