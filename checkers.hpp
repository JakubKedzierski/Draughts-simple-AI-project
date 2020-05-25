#pragma once
#include "board.hpp"
#include "checkersAI.hpp"

/* Klasa reprezentujaca prowadzon� gre - warcaby, reakcje u�ytkownika, status gry */
class checkers
{
private:
	RenderWindow window; // okno do renderowania grafiki
    BoardGraphic board;  // plansza
	Event event;		// zmienna do pobierania zdarzen
	Sprite image;		// zmienna do rysowania szachownicy
	bool Player;		// wybor gracza: true - biale , false - czarne 
	moveID moves;		// pobrane ruchy
	bool FirstTick = false, SecondTick = false; // zmienne monitoruj�ce zaznaczanie pionk�w przez u�ytkownika
	bool FewBeatings=false;         // zmienna monitoruj�ca czy na planszy s� wielokrotne bicia
public:
	BoardGraphic GetBoard() { return board; }
	
	/* Stworzenie gry */
	checkers() :board(true), window(VideoMode(WindowSize, WindowSize), "Warcaby"), Player(true) { window.setPosition(Vector2i(490, -8)); }
	
	/* Zainicjalizowanie gry*/
	void InitGame();
	void Play(); // p�tla g��wna z aktualizowaniem stanu rozgrywki
	void CheckForTicking(); // sprawdzanie interakcji z uzytkownikiem - zaznaczanie pionk�w
	void checkForGameOver(); //  sprawdzenie czy nast�pi� koniec gry
	void EndUpGame(bool); // ko�czenie gry
	void CheckForPlayer(moveID); // sprawdzania/zmiana gracza
	void moveCPU(); // ruch komputera/sztucznej inteligencji 

	// funkcje zwiazane z ruchem
	void Move(moveID);
	bool CheckAvailableMoves(moveID);
};
