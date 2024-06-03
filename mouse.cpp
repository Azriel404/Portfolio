#include "mouse.h"

Mouse::Mouse(GameSystem* game) :
	game(game),
	brush(game->GetBrushInstance()),
	field(game->GetFieldInstance()),
	command(game->GetCommandInstance()),
	character(game->GetCharacterInstance())
{
	mouseXpos = 0.0f;
	mouseYpos = 0.0f;
	mousePos = D2D1::Point2F(0.0f, 0.0f);

}


Mouse::~Mouse() {

}


void Mouse::OnMouseMove(float mouseXpos, float mouseYpos) {

	this->mouseXpos = mouseXpos;
	this->mouseYpos = mouseYpos;
	mousePos = D2D1::Point2F(mouseXpos, mouseYpos);

}


void Mouse::MouseClick_L() {

	if (mouseYpos >= command->GetCommandArea().top) {

		command->DetectCommand(mousePos);

	}
	else {

		if (command->GetIsSelected() != 999) {

			character->Generate(mouseXpos, mouseYpos, command->GetIsSelected());

			command->ResetIsSelected();

		}

	}

}


void Mouse::MouseClick_R() {

}


D2D1_POINT_2F Mouse::GetMousePos() {

	return mousePos;

}


float Mouse::GetMouseXPos() {

	return mouseXpos;

}


float Mouse::GetMouseYpos() {

	return mouseYpos;

}