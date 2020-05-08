#include "board.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

void moveID::TransformID() {
	x /= squareFrame; y /= squareFrame; x1 /= squareFrame; y1 /= squareFrame;
}
Board::Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].ChangeType(Empty);
		}
	}
}

Board::Board(bool mode){

	for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
			board[i][j].ChangeType(Empty);
        }
    }


    for(int i=1;i<8;i+=2){
		board[0][i] = Pawn(sf::Vector2f(0, i), BlackMan,mode);
		board[2][i] = Pawn(sf::Vector2f(2, i), BlackMan, mode);
		board[6][i] = Pawn(sf::Vector2f(6, i), WhiteMan, mode);
    }    

    for(int i=0;i<8;i+=2){
		board[1][i] = Pawn(sf::Vector2f(1, i), BlackMan, mode);
		board[5][i] = Pawn(sf::Vector2f(5, i), WhiteMan, mode);
		board[7][i] = Pawn(sf::Vector2f(7, i), WhiteMan, mode);
    }

	if (!scene.loadFromFile("szachownica.jpg")) {
		cerr << "Niepowodzenie wczytania pliku. Najprawdopodobniej w folderze z gra nie znajduje sie plik szachownica.jpg";
	}
}

void Board::PrintToStream(){
    for(int i=0;i<8;i++){
		cout << i << "  |";
		for(int j=0;j<8;j++){
			if (board[i][j].Type()) {
				cout << board[i][j].Type() << " ";
			}
			else {
				cout << "W" << " ";
			}
			}
        cout << endl;
    }
	cout << endl;
}

void Board::Move(moveID m){
	board[m.x1][m.y1] = board[m.x][m.y];
	board[m.x1][m.y1].GetTexture().setPosition(10 + m.y1 * 110.75, 10 + m.x1 * 110.75);
	board[m.x][m.y].ChangeType(Empty); 
	
	if (abs(m.y1 - m.y) == 2) {
		if ((m.y1 - m.y) > 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) > 0 && (m.x1 - m.x) < 0) { board[m.x - 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) < 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y - 1].ChangeType(Empty); }
		else{ board[m.x - 1][m.y - 1].ChangeType(Empty);}
	}
	
	if (m.x1 == 0 || m.x1 == 7) {
		board[m.x1][m.y1].Upgrade();
	}

}

vector<moveID> Board::PossibleMove(bool Player){
	vector<moveID> Possible;

	// 4 przypadki  ruchu

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j].Type() != Empty) {

				if ((board[i][j].side() && Player) || ((board[i][j].Type() == BlackKing) && !Player)) {
					if (i - 1 >= 0 && j + 1 < 8) {
						if (board[i - 1][j + 1].Type()==Empty) {
							Possible.push_back(moveID(i, j, i - 1, j + 1));
						}
					}
					if (i - 1 >= 0 && j - 1 >= 0) {
						if (board[i - 1][j - 1].Type() == Empty) {
							Possible.push_back(moveID(i, j, i - 1, j - 1));
						}
					}
				}

				if (((board[i][j].Type() == WhiteKing) && Player) || (!board[i][j].side() && !Player)) {
					if (i + 1 < 8 && j - 1 >= 0) {
						if (board[i + 1][j - 1].Type() == Empty) {
							Possible.push_back(moveID(i, j, i + 1, j - 1));
						}
					}
					if (i + 1 < 8 && j + 1 < 8) {
						if (board[i + 1][j + 1].Type() == Empty) {
							Possible.push_back(moveID(i, j, i + 1, j + 1));
						}
					}

				}
			}

		}
	}
	
	return Possible;
}

vector<moveID> Board::CheckForBeatings(bool Player) {
	vector<moveID> beating;
	
	// 8 przypadkow bicia

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j].Type() != Empty) {

				if ((board[i][j].side() && Player) || ((board[i][j].Type() == BlackKing) && !Player)) {
					if (i - 2 >= 0 && j + 2 < 8) {
						if (board[i - 2][j + 2].Type() == Empty && board[i - 1][j + 1].Type() != Empty) {

							if (Player) {
								if (!board[i - 1][j + 1].side()) {
									beating.push_back(moveID(i, j, i - 2, j + 2));
								}
							}
							else {
								if (board[i - 1][j + 1].side()) {
									beating.push_back(moveID(i, j, i - 2, j + 2));
								}
							}

						}
					}

					if (i - 2 >= 0 && j - 2 >= 0) {
						if (board[i - 2][j - 2].Type() == Empty && board[i - 1][j - 1].Type() != Empty) {

							if (Player) {
								if (!board[i - 1][j - 1].side()) {
									beating.push_back(moveID(i, j, i - 2, j - 2));
								}
							}
							else {
								if (board[i - 1][j - 1].side()) {
									beating.push_back(moveID(i, j, i - 2, j - 2));
								}
							}

						}
					}
				}

				if (((board[i][j].Type() == WhiteKing) && Player) || (!board[i][j].side() && !Player)) {

					if (i + 2 < 8 && j - 2 >= 0) {
						if (board[i + 2][j - 2].Type() == Empty && board[i + 1][j - 1].Type() != Empty) {

							if (Player) {
								if (!board[i + 1][j - 1].side()) {
									beating.push_back(moveID(i, j, i + 2, j - 2));
								}
							}
							else {
								if (board[i + 1][j - 1].side()) {
									beating.push_back(moveID(i, j, i + 2, j - 2));
								}
							}
						}
					}

					if (i + 2 < 8 && j + 2 < 8) {
						if (board[i + 2][j + 2].Type() == Empty && board[i + 1][j + 1].Type() != Empty) {

							if (Player) {
								if (!board[i + 1][j + 1].side()) {
									beating.push_back(moveID(i, j, i + 2, j + 2));
								}
							}
							else {
								if (board[i + 1][j + 1].side()) {
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

