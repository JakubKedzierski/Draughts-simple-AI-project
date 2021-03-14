#include "board.hpp"
#include <iostream>
using namespace std;

/* Przetworzenie graficznego ruchu na matematyczny */
void moveID::TransformID() {
	// wartosci dobrane eksperymentalna metoda poniewaz plansza ma ramke nalezy do poprawnego zoobrazowania pionkow
	// odj¹æ szerokoœæ ramki od pozycji
	x -= 101; y -= 101; x1 -= 101; y1 -= 101;  
	x /= SquareSize; y /= SquareSize; x1 /= SquareSize; y1 /= SquareSize;  // przetworzenie pozycji na pola planszy 8x8
}

/* Konstuktor bezparametryczny*/
BoardGraphic::BoardGraphic() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].ChangeType(Empty);
		}
	}
}

/* Tworzenie planszy na start gry */
BoardGraphic::BoardGraphic(bool mode){

	for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
			board[i][j].ChangeType(Empty); small.ChangeType(i, j, Empty);
        }
    }

	/* Stworzenie odpowiednich pionków w odpowiednich polach planszy matematycznej i graficznej*/

    for(int i=1;i<8;i+=2){
		board[0][i] = Pawn(sf::Vector2f(0, i), BlackMan); small.ChangeType(0, i, BlackMan);
		board[2][i] = Pawn(sf::Vector2f(2, i), BlackMan); small.ChangeType(2, i, BlackMan); 
		board[6][i] = Pawn(sf::Vector2f(6, i), WhiteMan); small.ChangeType(6, i, WhiteMan); 
    }    

    for(int i=0;i<8;i+=2){
		board[1][i] = Pawn(sf::Vector2f(1, i), BlackMan); small.ChangeType(1, i, BlackMan); 
		board[5][i] = Pawn(sf::Vector2f(5, i), WhiteMan); small.ChangeType(5, i, WhiteMan); 		
		board[7][i] = Pawn(sf::Vector2f(7, i), WhiteMan); small.ChangeType(7, i, WhiteMan); 
    }

	if (!scene.loadFromFile("szachownica1.jpg")) { // za³adowanie tekstury planszy
		cerr << "Niepowodzenie wczytania pliku. Najprawdopodobniej w folderze z gra nie znajduje sie plik szachownica.jpg";
	}
}

/* Wypisanie stanu planszy na stdout -> Uzywane pocz¹tkowo przy debugowaniu*/
void Board::PrintToStream(){
    for(int i=0;i<8;i++){
		cout << i << "  |";
		for(int j=0;j<8;j++){
			if (board[i][j]!=Empty) {
				cout << board[i][j] << " ";
			}
			else {
				cout << "W" << " ";
			}
			}
        cout << endl;
    }
	cout << endl;
}

/* Wykonanie ruchu pionkiem na planszy */
void Board::Move(moveID m) {
	board[m.x1][m.y1] = board[m.x][m.y]; // przypisanie do nowego pola
	board[m.x][m.y]=Empty; // poprzedni pole staje siê puste

	if (abs(m.y1 - m.y) == 2) { // gdy w ruchu bijemy pionka, nale¿y go usun¹æ z planszy
		if ((m.y1 - m.y) > 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y + 1]=Empty; }
		else if ((m.y1 - m.y) > 0 && (m.x1 - m.x) < 0) { board[m.x - 1][m.y + 1]=Empty;  }
		else if ((m.y1 - m.y) < 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y - 1]=Empty;  }
		else { board[m.x - 1][m.y - 1] = Empty;}
	}

	if (m.x1 == 0 || m.x1 == 7) { // awans
		if (board[m.x1][m.y1] == WhiteMan)board[m.x1][m.y1] = WhiteKing;
		if (board[m.x1][m.y1] == BlackMan)board[m.x1][m.y1] = BlackKing;
	}

}

/* Ruch reprezentowany graficznie */
void BoardGraphic::Move(moveID m){
	small.Move(m); 
	board[m.x1][m.y1] = board[m.x][m.y];
	// zmiana pozycji na planszy graficznej (dodaje wartoœæ szerokoœci ramki)
	board[m.x1][m.y1].GetTexture().setPosition(101 + m.y1 * SquareSize, 101 + m.x1 * SquareSize); 
	board[m.x][m.y].ChangeType(Empty); 
	
	if (abs(m.y1 - m.y) == 2) {  // gdy w ruchu bijemy pionka, nale¿y go usun¹æ z planszy
		if ((m.y1 - m.y) > 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) > 0 && (m.x1 - m.x) < 0) { board[m.x - 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) < 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y - 1].ChangeType(Empty); }
		else{ board[m.x - 1][m.y - 1].ChangeType(Empty);}
	}
	
	if (m.x1 == 0 || m.x1 == 7) {
		Upgrade(m.x1, m.y1); // awans zwyk³ego pionka na damkê
	}

}

/*
   Funkcja zwracaj¹ca mo¿liwe ruchy wybranego gracza przy danym statusie planszy
   Funkcja nie uwzglêdnia wystêpowania biæ -> od tego jest funkcja CheckForBeatings 
 */
vector<moveID> Board::PossibleMove(bool Player){
	vector<moveID> Possible; //kontener przechowuj¹cy mo¿liwe ruchy

	// 4 przypadki  ruchu -> lewo góra, lewo dó³, prawo góra, prawo dó³
	// dla odpowiednich warunków dodajemy odpowiedni ruchy do kontenera
	// i,j - reprezentuj¹ pola planszy

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] != Empty) {

				if ((side(i,j) && Player) || ((board[i][j] == BlackKing) && !Player)) {
					if (i - 1 >= 0 && j + 1 < 8) {   // prawo góra
						if (board[i - 1][j + 1]==Empty) {
							Possible.push_back(moveID(i, j, i - 1, j + 1));
						}
					}
					if (i - 1 >= 0 && j - 1 >= 0) { // lewo góra
						if (board[i - 1][j - 1] == Empty) {
							Possible.push_back(moveID(i, j, i - 1, j - 1));
						}
					}
				}

				if (((board[i][j] == WhiteKing) && Player) || (!side(i, j) && !Player)) {
					if (i + 1 < 8 && j - 1 >= 0) { // lewo dó³
						if (board[i + 1][j - 1] == Empty) {
							Possible.push_back(moveID(i, j, i + 1, j - 1));
						}
					}
					if (i + 1 < 8 && j + 1 < 8) { // prawo dó³
						if (board[i + 1][j + 1] == Empty) {
							Possible.push_back(moveID(i, j, i + 1, j + 1));
						}
					}

				}
			}

		}
	}
	
	return Possible;
}

/* Funkcja sprawdzaj¹ca czy na planszy s¹ jakieœ bicia dla danego gracza. Bicia w grze s¹ obowi¹zkowe*/
vector<moveID> Board::CheckForBeatings(bool Player) {
	vector<moveID> beating;
	
	// 4 przypadki bicia, w tym uwzglêdniam bicia damek w 4 strony, bicia pionków w 2 strony
	// i, j - pozycje na planszy

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] != Empty) {

				// Rozpatruje wszystkie przypadki mo¿liwego bicia, zarówno dla damek jak i pionków

				if ((side(i, j) && Player) || ((board[i][j] == BlackKing) && !Player)) {
					if (i - 2 >= 0 && j + 2 < 8) {// prawo góra
						
						if (board[i - 2][j + 2]== Empty && board[i - 1][j + 1] != Empty) {

							if (Player) {
								if (!side(i-1, j+1)) {
									beating.push_back(moveID(i, j, i - 2, j + 2));
								}
							}
							else {
								if (side(i - 1, j + 1)) {
									beating.push_back(moveID(i, j, i - 2, j + 2));
								}
							}

						}
					}

					if (i - 2 >= 0 && j - 2 >= 0) { // lewo góra
						if (board[i - 2][j - 2] == Empty && board[i - 1][j - 1] != Empty) {

							if (Player) {
								if (!side(i - 1, j - 1)) {
									beating.push_back(moveID(i, j, i - 2, j - 2));
								}
							}
							else {
								if (side(i - 1, j - 1)) {
									beating.push_back(moveID(i, j, i - 2, j - 2));
								}
							}

						}
					}
				}

				if (((board[i][j] == WhiteKing) && Player) || (!side(i, j) && !Player)) {

					if (i + 2 < 8 && j - 2 >= 0) { // lewo dó³
						if (board[i + 2][j - 2] == Empty && board[i + 1][j - 1] != Empty) {

							if (Player) {
								if (!side(i + 1, j - 1)) {
									beating.push_back(moveID(i, j, i + 2, j - 2));
								}
							}
							else {
								if (side(i + 1, j - 1)) {
									beating.push_back(moveID(i, j, i + 2, j - 2));
								}
							}
						}
					}

					if (i + 2 < 8 && j + 2 < 8) { // prawo dó³
						if (board[i + 2][j + 2] == Empty && board[i + 1][j + 1] != Empty) {

							if (Player) {
								if (!side(i + 1, j + 1)) {
									beating.push_back(moveID(i, j, i + 2, j + 2));
								}
							}
							else {
								if (side(i + 1, j + 1)) {
									beating.push_back(moveID(i, j, i + 2, j + 2));
								}
							}

						}
					}
				}

			}
		}
	}
	return beating;
}

/* Sprawdzenie czy na planszy jest jakiœ pionek danego typu */
bool Board::IsAny(bool type) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (type) {
				if (board[i][j] == WhiteMan || board[i][j] == WhiteKing) return true;
			}
			else {
				if (board[i][j] == BlackMan || board[i][j] == BlackKing) return true;
			}
		}
	}
	return false;
}

