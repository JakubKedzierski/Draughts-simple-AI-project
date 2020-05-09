#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
//const int WindowSize = 886;
//const double SquareSize = 886 / 8;
const double WindowSize = 1000;
const double SquareSize = 815 / 8;

using namespace std;
using namespace sf;

enum PawnType {Empty, BlackMan, BlackKing, WhiteMan, WhiteKing };

class Pawn {
	int x, y;
	PawnType type;
	Texture *texture;
	CircleShape shape;
public:
	Pawn(){}
	Pawn(Vector2f, PawnType,bool);
	PawnType Type() const { return type; }
	void ChangeType(PawnType t) { type = t; }
	CircleShape &GetTexture() { return shape; }
	bool side() { if (type == BlackMan || type == BlackKing) return false; else return true; }
	void tick() {
		shape.setOutlineColor(sf::Color(0, 255, 0)); shape.setOutlineThickness(10);
	}
	void untick() { shape.setOutlineThickness(0); }
};