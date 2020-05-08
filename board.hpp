#pragma once
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Pawn.hpp"
#include <chrono>
using namespace std::chrono;
using namespace std;
using namespace sf;

struct moveID {
private:
	int squareFrame;
public:
	int x, y, x1, y1;
	
	moveID(int X, int Y, int X1, int Y1) { x = X; y = Y; x1 = X1; y1 = Y1; squareFrame= SquareSize;}
	moveID() { squareFrame = SquareSize; }
	bool operator==(const moveID &m) { if (m.x == x && m.y == y && m.x1 == x1 && m.y1 == y1) { return true; } else { return false; } }
	void TransformID();
};



class Board{
    Pawn board[8][8]; 
	Texture scene;
public:
    Board();
	Board(bool);
    vector<moveID> PossibleMove(bool);
    void Move(moveID);
    void PrintToStream();
    Pawn &GetPawn(int x,int y){return board[x][y];}
	Pawn GetPawn(int x, int y)const{ return board[x][y]; }
	vector<moveID> CheckForBeatings(bool);
	Texture &GetTexture() { return scene; }
};

/*
class graphics {
	Board board[8][8];
	Texture *texture[2];
	CircleShape shape[4];
public:

};
*/