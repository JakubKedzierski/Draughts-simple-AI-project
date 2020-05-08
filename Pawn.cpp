#include "Pawn.hpp"
using namespace std;
using namespace sf;

Pawn::Pawn(sf::Vector2f pos, PawnType type1,bool mode) {
	x = pos.x; y = pos.y;
	type = type1;
	if (mode) {
		shape.setRadius(45);
		shape.setPosition(10 + y * 110.75, 10 + x * 110.75);
		switch (type1) {
		case BlackMan: {
			shape.setFillColor(sf::Color(255, 0, 0));
			break;
		}

		case WhiteMan: {
			shape.setFillColor(sf::Color(255, 255, 255));
			break;
		}

		case WhiteKing: {
			break;
		}

		case BlackKing: {
			break;
		}

		}
	}
}

void Pawn::Upgrade() {
	if (type == BlackMan) {
		type = BlackKing;
		texture = new Texture();
		if (!texture->loadFromFile("blackking2.jpg"))
			cerr << "Nie udalo sie zaladowac blackkinga";
		shape.setTexture(texture);
	}
	else if (type == WhiteMan) {
		type = WhiteKing;
		texture = new Texture();
		if (!texture->loadFromFile("whiteking.png"))
			cerr << "Nie udalo sie zaladowac whitekinga";
		shape.setTexture(texture);
	}

}