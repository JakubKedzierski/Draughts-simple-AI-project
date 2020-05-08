#include "checkers.hpp"
#include <iostream>

using namespace std;
using namespace sf;

void checkers::InitGame() {
	while (window.isOpen())
	{
		window.clear();
		image.setTexture(board.GetTexture());
		window.draw(image);
		Play();
	}
}

void checkers::checkForGameOver() {
	bool iterator1=false,iterator2=false;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board.GetPawn(i,j).Type() != Empty) {
				if (board.GetPawn(i, j).side()) {
					iterator1 = true;
				}
				else {
					iterator2 = true;
				}
			}
		}
	}

	if ( (!iterator1) || (!iterator2) ) EndUpGame();
}

void checkers::EndUpGame() {
	RenderWindow EndWin(VideoMode(500, 500), "Warcaby ");
	Text endText;
	sf::Font font;
	
	if (!font.loadFromFile("font.ttf"))cerr << " Brak pliku font.ttf w dysku z gra";
	
	endText.setFont(font);
	endText.setString("Koniec gry !!! ");
	endText.setCharacterSize(20);
	endText.setFillColor(sf::Color::Red);

	while (EndWin.isOpen()) {
		while (EndWin.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				EndWin.close();
			}
			EndWin.clear(Color(255,255,255));
			EndWin.draw(endText);
			EndWin.display();

		}
	}
}

void checkers::CheckForTicking() {
	if (event.type == Event::MouseButtonReleased) {
		if (!FirstTick && !SecondTick) {
			moves.x = Mouse::getPosition(window).y;
			moves.y = Mouse::getPosition(window).x;
			int x = moves.x / (WindowSize / 8), y = moves.y / (WindowSize / 8); // ustalenie polozenia na szachownicy

			if (board.GetPawn(x, y).Type() != Empty)
				board.GetPawn(x, y).tick();

			FirstTick = true;
		}
		else if (FirstTick) {
			moves.x1 = Mouse::getPosition(window).y;
			moves.y1 = Mouse::getPosition(window).x;
			SecondTick = true;
		}

	}
}

void checkers::Play(){
			
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
			
		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
			window.close();

		CheckForTicking();
	}

	if (FirstTick&&SecondTick) {
		FirstTick = false; SecondTick = false;
		Move(moves);
		checkForGameOver();
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board.GetPawn(i, j).Type() != Empty) {
				window.draw(board.GetPawn(i, j).GetTexture());
			}
		}
	}

	if (!Player) {
		moveCPU();
	}

     window.display();
}

void checkers::Move(moveID m) {
	m.TransformID();

	if(board.GetPawn(m.x, m.y).Type() != Empty)
	board.GetPawn(m.x, m.y).untick();
	
	if (m.x != m.x1 && m.y != m.y1)	{
		if (CheckAvailableMoves(m)){	
			board.Move(m);
			CheckForPlayer();
		}
	}
}

bool checkers::CheckAvailableMoves(moveID m) {
	vector<moveID> Possible;
	
	Possible = board.CheckForBeatings(Player);  // gdy sa jakies bicia sa one obowiazkowe
	if (Possible.size() > 0) FewBeatings = true; else FewBeatings = false;

	if (!Possible.size()) {
		if (Player) {
			Possible = board.PossibleMove(true);
		}
		else {
			Possible = board.PossibleMove(false);
		}
	}

	for (int i=0;i<Possible.size();i++)
	{
		if (Possible[i] == m) {
			Possible.clear();
			return true;
		}
	}

	Possible.clear();
	return false;
}

void checkers::CheckForPlayer() {
	vector<moveID> Possible;
	Possible = board.CheckForBeatings(Player);
	if (!(FewBeatings && Possible.size() > 0))  Player = !Player;
}

void checkers::moveCPU() {
	vector<moveID> Possible;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	Possible = board.CheckForBeatings(Player);  // gdy sa jakies bicia sa one obowiazkowe
	if (Possible.size() > 0) {
		FewBeatings = true;
	}
	else {
		FewBeatings = false;
		Possible = board.PossibleMove(Player);
	}

	Pawn* p;
	for(int i=100;i>0;i--) p=new Pawn(sf::Vector2f(6, i), WhiteMan,false);
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	cout << time_span.count() * 1000 << endl;

	checkersAI ai;
	ai.GetAIindexToMove(board);
	
	if(Possible.size()!=0)
	board.Move(Possible[rand()%Possible.size()]);
	
	Possible.clear();
	CheckForPlayer();
		
}