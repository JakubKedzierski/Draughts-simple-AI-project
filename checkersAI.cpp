#include "checkersAI.hpp"

/*Funckja zwracajaca najlepszy ruch wybrany przez sztuczną inteligencje*/
moveID GetAImove(const Board board) {
	
	// Tworzenie drzewa rozwiazan na x ruchów
	TreeNode tree(5,board);
	// Ruch wybrany przez sztuczna inteligencje (domyślnie wyrzucam wyjątek, na wypadek nieprzewidzianych sytuacji)
	moveID move(0, 0, 0, 0);
	
	int choice=-Infty; // Inicjalizacja wyboru najgorszą możliwością dla komputera
	int tmp; // zmienna pomocnicza
	
	for (auto son : tree.sons) {
		tmp = minmax(son);			// dla każdego syna początkowego wierzchołka przeprowadzam algorytm minmax
		
		if (tmp >= choice) {
			choice = tmp;
			move = son.move;		// ostateczny wybór ruchu
		}

	}

	return move;
}

/* Algorytm SI - minmax*/
int minmax(TreeNode tree) {
	
	TreeNode ret;
	int choice;

	// Przypisanie odpowienich wartości do wyboru (-oo dla maksymalizowanego gracza i oo dla przeciwnika)
	if (tree.Player) {
		choice = -Infty; 
	}
	else {
		choice = Infty;
	}
	

	for (auto son : tree.sons) {    
		
		if (son.Player) {
			if (son.heuristicGrade <= choice) {
				choice = min(son.heuristicGrade, choice);  // wybor minimalny prowadzacy do mniejszej wartosci koncowej
				ret = son;
			}
		}

		if (!son.Player) {
			if (son.heuristicGrade >= choice) {
				choice = max(son.heuristicGrade, choice); // wybor maksymalny prowadzacy do wiekszej wartosci koncowej
				ret = son;
			}
		}
	}

	if (tree.sons.size()) {
		if (tree.sons[0].depth > 0) {  // dopoki glebokosc w wierzcholku syna > 0
			choice += minmax(ret);     // powtarzamy algorytm
		}
	}

	return choice;
}

/* Tworzenie synów w danym wierzchołku */
void TreeNode::MakeSons() {
	vector<moveID> moves; // ruch przeprowadzany w danym wierzchołku
	bool moreBeats=false; // zmienna pomocnicza do sprawdzania wielokrotnnego bicia
	
	moves = boardStatus.CheckForBeatings(Player);  // sprawdzenie czy są bicia (obowiązkowe)
	if (moves.size() > 0) {
		moreBeats = true;
	}
	else {
		moreBeats = false;
		moves = boardStatus.PossibleMove(Player);
	}

	if (moves.size() == 0 && Player) { heuristicGrade = Infty; return; } // stany końcowe w grze 
	else if (moves.size() == 0) { heuristicGrade = -Infty; return;}

	
	for (auto &choice : moves) { // wszystkie dostepne ruchy to nowe wierzchołki w drzewie
		TreeNode temp;
		temp = *this;         
		temp.sons.clear();
		temp.boardStatus.Move(choice);   // wykonanie ruchu na analizowanym statusie planszy
		temp.depth--;                    // po kazdym ruchu obnizamy glebokosc, aby w koncu dojsc do liści
		temp.move = choice;
		temp.heuristicGrade = heuristicForCheckers(temp.boardStatus, temp.Player); // ocena wartosci planszy funckją oceniającą
		
		temp.CheckForPlayer(move);      // sprawdzenie czy po ruchu nastąpi zmiana gracza
		temp.FewBeatings = moreBeats;	// przypisanie na wypadek wielokrotnego bicia
		sons.push_back(temp);   // dodawanie nowych wierzchołków do drzewa
		
		if (temp.depth > 0) {
			sons.back().MakeSons();   // jeśli nie doszliśmy do liści tworzymy nowych synów
		}

	}
	moves.clear();
}


/* Sprawdzenie czy po ruchu następuje zmiana gracza*/
void TreeNode::CheckForPlayer(moveID prevMove) {
	vector<moveID> Possible;
	Possible = boardStatus.CheckForBeatings(Player);
	for (auto newBeat : Possible) {
		if (prevMove.y1 == newBeat.y && prevMove.x1 == newBeat.x) { 
			if (FewBeatings) { // gdy po ruchu w którym było bicie, możliwe są dalsze bicia zmiana gracza nie następuje
				return;
			}
		}
	}
	Player = !Player;
}


// Funkcja oceniająca wartośc stanu gry na podstawie danego stanu planszy
// W argumentach przekazywany stan planszy oraz dla jakiego gracza dokonujemy oceny
int heuristicForCheckers(Board board, bool Player) {
	// Ocene wartości danego stanu na planszy dokonuje oceniajać 2 kryteria:
	// - polozenie pionka na planszy -> wyróżniam ścisłe centrum (5pkt), slabsze centrum (3pkt) oraz obrzeża (0pkt)
	// - ilość pionków danego rodzaju na planszy -> pionek (40pkt) , damka (100pkt)
	// - położenie pionka na jednym z dwóch poziomów -> 10pkt I poziom, 5pkt II poziom
	// Wartości zostały dobrane na podstawie eksperymentów oraz po przeanalizowaniu strategi stosowanych w warcabach
	// przez graczy.
	
	int value = 0; // zwracana wartość stanu gry
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			
			if (board(i,j)!=Empty) {  
				switch (board(i, j)) {

				case WhiteMan: {
					if (i < 2) value -= 10;
					if (i < 4 && i > 1) value -= 5;
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value -= 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value -= 5;
					value -= 40;
					break;
				}

				case WhiteKing: {
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value -= 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value -= 5;
					value -= 100;
					break;
				}
				}
				
				switch (board(i, j)) {
				case BlackKing: {
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value += 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value += 5;
					value += 100;
					break;
				}

				case BlackMan: {
					if (i > 5) value += 10;
					if (i>3 && i < 5) value += 5;
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value += 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value += 5;
					value += 40;
					break;
				}
				
				}
			}

		}
	}
	return value;
}
