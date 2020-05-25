#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Pawn.hpp"
using namespace sf;

/* Struktura przechowuj¹ca dane o ruchu wykonywanym w grze*/
struct moveID {
public:
	int x, y, x1, y1; // wspó³rzêdne ruchu ( x,y pionek który siê rusza, x1 y1 miejsce docelowe)
	
	moveID(int X, int Y, int X1, int Y1) { x = X; y = Y; x1 = X1; y1 = Y1;}
	moveID() { }

	// porównanie czy ruchy s¹ identyczne
	bool operator==(const moveID &m) { if (m.x == x && m.y == y && m.x1 == x1 && m.y1 == y1) { return true; } else { return false; } } 
	void TransformID(); // funkcja do transformowania pozycji pionka z graficznego okna do planszy 8x8
};

// Klasa reprezentuj¹ca szachownicê obliczeniow¹ bez graficznej oprawy.
// Zrobi³em podzia³ na dwie plansze, aby odzieliæ wartstwê graficzn¹ od "matematycznej"
class Board {
	PawnType board[8][8]; // plansza reprezentowana jako tablica 8x8
public:	
	void Move(moveID m); // wykonanie ruchu pionkiem
	void PrintToStream(); // wypisanie stanu planszy na stdout w konsoli
	vector<moveID> CheckForBeatings(bool); // funkcja zwracaj¹ca mo¿liwe bicia
	vector<moveID> PossibleMove(bool);		// funkcja zwracaj¹ca mo¿liwe ruchy (bez biæ)
	PawnType operator()(int i, int j) { return board[i][j]; } // dostêp do pola
	void ChangeType(int i, int j, PawnType type) { board[i][j] = type; } // zmiana rodzaju pionka (przy awansie)
	bool side(int i,int j) { if (board[i][j] == BlackMan || board[i][j] == BlackKing) return false; else return true; }
	bool IsAny(bool type); // sprawdzenie czy na planszy s¹ jeszcze jakieœ pionki danego gracza
};


/* Plansza odpowiadaj¹ca za warstwê graficzn¹*/
class BoardGraphic{ 
	Board small;      // matematyczny opis planszy
	Pawn board[8][8]; // graficzny opisz planszy
	Texture scene;    // tekstura odpowiadajaca za wyglad planszy
public: 
	BoardGraphic();
	BoardGraphic(bool);

	Board mathBoard()const { return small; }
	Pawn &GetPawn(int x, int y) { return board[x][y]; }
	Pawn GetPawn(int x, int y)const { return board[x][y]; }
	Texture &GetTexture() { return scene; }
    void Move(moveID); // ruch na graficznej planszy
	void Upgrade(int i, int j) { // awans na graficznej planszy
		if (board[i][j].Type() == BlackMan) { board[i][j]= Pawn(sf::Vector2f(i, j), BlackKing);}
		else if(board[i][j].Type() == WhiteMan){ board[i][j] = Pawn(sf::Vector2f(i, j), WhiteKing); }
	}
};

