#include "checkers.hpp"


void checkers::InitGame() {
	while (window.isOpen()) // otwarcie okna 
	{
		window.clear();
		image.setTexture(board.GetTexture()); // za�adowanie szachownicy
		window.draw(image);
		Play();     // rozpocz�cie gry
	}
}

/* P�tla g��wna z gr� */
void checkers::Play() {

	while (window.pollEvent(event)) // sprawdzanie zdarze� m.in zamkni�cie okna, czy zaznaczenie pionka
	{
		if (event.type == Event::Closed)
			window.close();

		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
			window.close();

		CheckForTicking();
	}

	if (!Player) {
		moveCPU(); // ruch komputera -> czarne 
		checkForGameOver();
	}

	if (FirstTick&&SecondTick) {
		FirstTick = false; SecondTick = false;
		Move(moves);   // ruch u�ytkownika 
		checkForGameOver();
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board.GetPawn(i, j).Type() != Empty) {
				window.draw(board.GetPawn(i, j).GetTexture()); // rysowanie pionk�w na planszy
			}
		}
	}

	window.display();
}

/* 
Sprawdzenie czy na planszy s� jeszcze pionki danego gracza. Dodatkowo analizowany jest te� przypadek
gdy gra ko�czy si� w wyniku braku ruchu przeciwnika
*/
void checkers::checkForGameOver() {
	if (!board.mathBoard().IsAny(true)) EndUpGame(false);

	if (!board.mathBoard().IsAny(false)) EndUpGame(true);

}

/* Zako�czenie gry */
void checkers::EndUpGame(bool endStatus) {
	RenderWindow EndWin(VideoMode(500, 500), "Warcaby "); // Stworzenie nowego okna ko�cz�cego rozgrywk�
	Text endText;
	sf::Font font;
	
	if (!font.loadFromFile("font.ttf"))cerr << " Brak pliku font.ttf w dysku z gra"; // za�adowanie czcionki
	
	endText.setFont(font);
	if (endStatus) {
		endText.setString("Koniec gry !!! Wygraly biale "); 
	}
	else {
		endText.setString("Koniec gry !!! Wygraly czarne ");
	}

	endText.setCharacterSize(20);
	endText.setFillColor(sf::Color::Red);

	while (EndWin.isOpen()) { // otwarcie okna i zako�czenie rozgrywki
		while (EndWin.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				EndWin.close();
			}
			EndWin.clear(Color(255,255,255));
			EndWin.draw(endText);// wypisanie w oknie powiadomienia
			EndWin.display();

		}
	}
}

/* Sprawdzenie czy u�ytkownik zaznaczy� jakiego� pionka */
void checkers::CheckForTicking() {
	if (event.type == Event::MouseButtonReleased) {
		if (!FirstTick && !SecondTick) {             // po klikni�ciu pobieram pozycje myszki 
			moves.x = Mouse::getPosition(window).y;
			moves.y = Mouse::getPosition(window).x;

			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
						board.GetPawn(i, j).tick(moves.x,moves.y); // gdy u�ytkownik klikn�� pionka zostaje on zaznaczony
				}
			}
			FirstTick = true;
		}
		else if (FirstTick) { // po pierwszym klikni�ciu czekamy na drugie kt�re pokazuje w kt�re miejsce pionek ma si� przemie�ci�
			moves.x1 = Mouse::getPosition(window).y;
			moves.y1 = Mouse::getPosition(window).x;
			SecondTick = true;
		}

	}
}

/* Sprawdzenie czy ruch u�ytkownika jest mo�liwy do wykonania a nast�pnie wykonanie go*/
void checkers::Move(moveID m) {
	m.TransformID(); // transformowanie pozycji myszki na pozycj� na planszy 8x8

	if(board.GetPawn(m.x, m.y).Type() != Empty)
	board.GetPawn(m.x, m.y).untick(); // odznaczenie pionka 
	
	if (m.x != m.x1 && m.y != m.y1)	{
		if (CheckAvailableMoves(m)){	// sprawdzenie dost�pnych ruch�w i wykonanie ruchu
			board.Move(m);
			CheckForPlayer(m); // zmiana gracza gdy nie ma wielokrotnych bi� 
		}
	}
}

/* Sprawdzenie czy ruch zaznaczony przez u�ytkownika jest dost�pnym ruchem
	- gdy jest jakie� bicie jest ono obowi�zkowe
@param m -> ruch zaznaczony przez u�ytkownika
*/
bool checkers::CheckAvailableMoves(moveID m) {
	vector<moveID> Possible;
	
	Possible = board.mathBoard().CheckForBeatings(Player);  // gdy sa jakies bicia sa one obowiazkowe
	if (Possible.size() > 0) FewBeatings = true; else FewBeatings = false;

	if (!Possible.size()) {
		if (Player) {
			Possible = board.mathBoard().PossibleMove(true);
		}
		else {
			Possible = board.mathBoard().PossibleMove(false);
		}
	}
	if (Possible.size() == 0) EndUpGame(false); // gdy gracz nie ma ju� dost�pnego ruchu koniec gry

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

void checkers::CheckForPlayer(moveID prevMove) {
	vector<moveID> Possible;
	Possible = board.mathBoard().CheckForBeatings(Player);
	
	for (auto newBeat : Possible) {
		if (prevMove.y1 == newBeat.y && prevMove.x1 == newBeat.x) { 
			if (FewBeatings) { // w przypadku wielokrotnego bicia nie zmieniamy gracza
				return;
			}
		}
	}
	Player = !Player;
}

/* Ruch komputera/ sztucznej inteligencji */
void checkers::moveCPU() {

	vector<moveID> Possible;
	Possible = board.mathBoard().CheckForBeatings(Player); // sprawdzenie czy s� jakie� bicia dla komputera 
														// ( na wypadek wielokrotnego bicia, wtedy nie zmieniam gracza )
	if (Possible.size() > 0) FewBeatings = true; else FewBeatings = false;

	moveID m = GetAImove(board.mathBoard()); // pozyskanie ruchu sztucznej inteligencji 
	board.Move(m);

	CheckForPlayer(m);
}