#include "board.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

void moveID::TransformID() {
	x -= 101; y -= 101; x1 -= 101; y1 -= 101;
	x /= squareFrame; y /= squareFrame; x1 /= squareFrame; y1 /= squareFrame;
}
BoardGraphic::BoardGraphic() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].ChangeType(Empty);
		}
	}
}

BoardGraphic::BoardGraphic(bool mode){

	for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
			board[i][j].ChangeType(Empty); small.board[i][j] = Empty;
        }
    }

    for(int i=1;i<8;i+=2){
		board[0][i] = Pawn(sf::Vector2f(0, i), BlackMan,mode); small.board[0][i] = BlackMan;
		board[2][i] = Pawn(sf::Vector2f(2, i), BlackMan, mode); small.board[2][i] = BlackMan;
		board[6][i] = Pawn(sf::Vector2f(6, i), WhiteMan, mode); small.board[6][i] = WhiteMan;
    }    

    for(int i=0;i<8;i+=2){
		board[1][i] = Pawn(sf::Vector2f(1, i), BlackMan, mode); small.board[1][i] = BlackMan;
		board[5][i] = Pawn(sf::Vector2f(5, i), WhiteMan, mode); small.board[5][i] = WhiteMan;
		board[7][i] = Pawn(sf::Vector2f(7, i), WhiteMan, mode); small.board[7][i] = WhiteMan;
    }

	if (!scene.loadFromFile("szachownica1.jpg")) {
		cerr << "Niepowodzenie wczytania pliku. Najprawdopodobniej w folderze z gra nie znajduje sie plik szachownica.jpg";
	}
}


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

void Board::Move(moveID m) {
	board[m.x1][m.y1] = board[m.x][m.y];
	board[m.x][m.y]=Empty;

	if (abs(m.y1 - m.y) == 2) {
		if ((m.y1 - m.y) > 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y + 1]=Empty; }
		else if ((m.y1 - m.y) > 0 && (m.x1 - m.x) < 0) { board[m.x - 1][m.y + 1]=Empty;  }
		else if ((m.y1 - m.y) < 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y - 1]=Empty;  }
		else { board[m.x - 1][m.y - 1] = Empty;}
	}

	if (m.x1 == 0 || m.x1 == 7) {
		if (board[m.x1][m.y1] == WhiteMan)board[m.x1][m.y1] = WhiteKing;
		if (board[m.x1][m.y1] == BlackMan)board[m.x1][m.y1] = BlackKing;
	}

}

void BoardGraphic::Move(moveID m){
	small.Move(m);
	board[m.x1][m.y1] = board[m.x][m.y];
	board[m.x1][m.y1].GetTexture().setPosition(101 + m.y1 * SquareSize, 101 + m.x1 * SquareSize);
	board[m.x][m.y].ChangeType(Empty); 
	
	if (abs(m.y1 - m.y) == 2) {
		if ((m.y1 - m.y) > 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) > 0 && (m.x1 - m.x) < 0) { board[m.x - 1][m.y + 1].ChangeType(Empty); }
		else if((m.y1 - m.y) < 0 && (m.x1 - m.x) > 0) { board[m.x + 1][m.y - 1].ChangeType(Empty); }
		else{ board[m.x - 1][m.y - 1].ChangeType(Empty);}
	}
	
	if (m.x1 == 0 || m.x1 == 7) {
		Upgrade(m.x1, m.y1);
		//board[m.x1][m.y1].Upgrade();
	}

}

vector<moveID> Board::PossibleMove(bool Player){
	vector<moveID> Possible;

	// 4 przypadki  ruchu

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] != Empty) {

				if ((side(i,j) && Player) || ((board[i][j] == BlackKing) && !Player)) {
					if (i - 1 >= 0 && j + 1 < 8) {
						if (board[i - 1][j + 1]==Empty) {
							Possible.push_back(moveID(i, j, i - 1, j + 1));
						}
					}
					if (i - 1 >= 0 && j - 1 >= 0) {
						if (board[i - 1][j - 1] == Empty) {
							Possible.push_back(moveID(i, j, i - 1, j - 1));
						}
					}
				}

				if (((board[i][j] == WhiteKing) && Player) || (!side(i, j) && !Player)) {
					if (i + 1 < 8 && j - 1 >= 0) {
						if (board[i + 1][j - 1] == Empty) {
							Possible.push_back(moveID(i, j, i + 1, j - 1));
						}
					}
					if (i + 1 < 8 && j + 1 < 8) {
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

vector<moveID> Board::CheckForBeatings(bool Player) {
	vector<moveID> beating;
	
	// 8 przypadkow bicia

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] != Empty) {

				if ((side(i, j) && Player) || ((board[i][j] == BlackKing) && !Player)) {
					if (i - 2 >= 0 && j + 2 < 8) {
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

					if (i - 2 >= 0 && j - 2 >= 0) {
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

					if (i + 2 < 8 && j - 2 >= 0) {
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

					if (i + 2 < 8 && j + 2 < 8) {
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

