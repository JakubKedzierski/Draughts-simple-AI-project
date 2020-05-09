#include "Pawn.hpp"
using namespace std;
using namespace sf;

Pawn::Pawn(sf::Vector2f pos, PawnType type1,bool mode) {
	x = pos.x; y = pos.y;
	type = type1;
	if (mode) {
		shape.setRadius(45);
		shape.setPosition(101 + y * SquareSize, 101 + x * SquareSize);
		switch (type1) {
		case BlackMan: {
			texture = new Texture();
			if (!texture->loadFromFile("blackpawn.png"))
				cerr << "Nie udalo sie zaladowac whitemana";
			shape.setTexture(texture);
			break;
		}

		case WhiteMan: {
			texture = new Texture();
			if (!texture->loadFromFile("whitepawn.png"))
				cerr << "Nie udalo sie zaladowac whitemana";
			shape.setTexture(texture);
			break;
		}

		case WhiteKing: {
			texture = new Texture();
			if (!texture->loadFromFile("whiteking.png"))
				cerr << "Nie udalo sie zaladowac whitekinga";
			shape.setTexture(texture);
			break;
		}

		case BlackKing: {
			texture = new Texture();
			if (!texture->loadFromFile("blackking.png"))
				cerr << "Nie udalo sie zaladowac whitekinga";
			shape.setTexture(texture);
			break;
		}

		}
	}
}
