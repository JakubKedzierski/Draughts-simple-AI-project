#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Pawn.hpp"
using namespace std;
using namespace sf;

struct moveID {
public:
	int x, y, x1, y1;
	moveID(int X, int Y, int X1, int Y1) { x = X; y = Y; x1 = X1; y1 = Y1;}
	moveID() { }
	bool operator==(const moveID &m) { if (m.x == x && m.y == y && m.x1 == x1 && m.y1 == y1) { return true; } else { return false; } }
	void TransformID();
};


class Board { // szachownica obliczeniowa bez graficznej oprawy
	PawnType board[8][8];
public:	
	void Move(moveID m);
	void PrintToStream();
	vector<moveID> CheckForBeatings(bool);
	vector<moveID> PossibleMove(bool);
	PawnType operator()(int i, int j) { return board[i][j]; }
	void ChangeType(int i, int j, PawnType type) { board[i][j] = type; }
	bool side(int i,int j) { if (board[i][j] == BlackMan || board[i][j] == BlackKing) return false; else return true; }
	bool IsAny(bool type);
};

class BoardGraphic{ // tu by sie grafika robila
	Board small;
	Pawn board[8][8]; 
	Texture scene;
public: 
	Board mathBoard()const { return small; }
	BoardGraphic();
	BoardGraphic(bool);
    void Move(moveID);
    Pawn &GetPawn(int x,int y){return board[x][y];}
	Pawn GetPawn(int x, int y)const{ return board[x][y]; }
	Texture &GetTexture() { return scene; }
	void Upgrade(int i, int j) { 
		if (board[i][j].Type() == BlackMan) { board[i][j]= Pawn(sf::Vector2f(i, j), BlackKing);}
		else if(board[i][j].Type() == WhiteMan){ board[i][j] = Pawn(sf::Vector2f(i, j), WhiteKing); }
	}
};

