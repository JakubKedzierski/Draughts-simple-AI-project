#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// rozmiar okna ( przypisany na sta�e ), jest to nieeleganckie rozwi�zanie, 
// jednak zastosowa�em je poniewa� nie skupia�em si� na "szacie graficznej" programu
// Dodatkowo mam bardzo ma�� wiedz� na temat tworzenia program�w graficznych
const double WindowSize = 1000;     
// Rozmiar pola na szachownicy
const double SquareSize = 815 / 8;


/* Rodzaj pionka na planszy: puste pole -brak pionka,
	czarny pionek, czarna damka, bia�y pionek, bia�a damka*/
enum PawnType {Empty, BlackMan, BlackKing, WhiteMan, WhiteKing };

/*Klasa Pawn obrazuje wersje graficzna pionka, jego wyglad pozycje na planszy (graficznej), 
	zaznaczenie (przy zaznaczaniu ruchu ) oraz pozwala na ewentualne zmiany wygladu/ pozycji */
class Pawn {
	PawnType type;  // rodzaj pionka
	Texture *texture; // tekstura do wygladu
	CircleShape shape; // ksztalt pionka - wszystkie pionki sa okragle
public:
	Pawn(){}
	Pawn(Vector2f, PawnType);
	
	/* Metody dostepu do danych pionka*/
	PawnType Type() const { return type; }
	CircleShape &GetTexture() { return shape; }
	bool side() { if (type == BlackMan || type == BlackKing) return false; else return true; }

	void ChangeType(PawnType t) { type = t; } // zmiana typu pionka (np przy awansie)
	
	/* Zaznaczanie pionka */
	void tick(double x,double y) { 
		if (shape.getGlobalBounds().contains(Vector2f(y, x))) {
			shape.setOutlineColor(sf::Color(0, 255, 0)); shape.setOutlineThickness(10);
		}
	}

	/* Odznaczanie pionka */
	void untick() { shape.setOutlineThickness(0); }
};