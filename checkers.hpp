#pragma once
#include "board.hpp"
#include <SFML/Graphics.hpp>
#include "checkersAI.hpp"
using namespace sf;

class checkers
{
private:
	RenderWindow window; // okno do renderowania grafiki
    Board board; // plansza
	Event event; // zmienna do pobierania zdarzen
	Sprite image; // zmienna do rysowania szachownicy
	bool Player; // wybor gracza: true - biale , false - czarne (czerwone)
	moveID moves; // podbrane ruchy
	bool FirstTick = false, SecondTick = false;
	bool FewBeatings=false;
public:
	Board GetBoard() { return board; }
	checkers() :board(true), window(VideoMode(WindowSize, WindowSize), "Warcaby"), Player(true) {}
	
	void InitGame();
	void Play();
	void CheckForTicking();
	void checkForGameOver();
	void EndUpGame();
	void CheckForPlayer();
	void moveCPU();

	// funkcje zwiazane z ruchem
	void Move(moveID);
	bool CheckAvailableMoves(moveID);

};
