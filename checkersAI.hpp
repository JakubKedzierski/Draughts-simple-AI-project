#pragma once
#include "board.hpp"
#include <vector>
const long int Infty = 9999999;

// Wierzcholki drzewa mo�liwych rozwi�za� gry (ruch�w do przodu)
class TreeNode {
public:
	/* Informacje przechowywane w wierzcho�ku drzewa : */

	int heuristicGrade;  // warto�� danego wierzcho�ka obliczona przez funkcje heurystyczna / ocena danego ruchu
	Board boardStatus;   // status na planszy
	moveID move;		 // ruch kt�ry jest analizowany w danym wierzcho�ku
	bool Player;		 // gracz kt�ry rusza si� w danym wierzcho�ku
	int depth;			 // liczba krokow liczona do przodu od danego wierzcho�ka
	vector<TreeNode> sons;  // "synowie" wierzcho�ka - mo�liwe rozwi�zania gry po wykonaniu ruchu w tym wierzcho�ku
	bool FewBeatings;	 // zmienna do zapami�tywania ewentualnego wielokrotnego bicia
	
	/***************************************************/
	
	TreeNode() {}

	/* Tworzenie drzewa mo�liwych rozwi�za� */
	TreeNode(int steps, Board b) {
		boardStatus = b; depth = steps; Player = false; FewBeatings = false;  
		MakeSons();
	}
	
	void MakeSons(); // Tworzenie "syn�w " czyli kolejnych krok�w, mo�liwych rozwi�za� gry
	void CheckForPlayer(moveID prevMove); // Zmiana gracza (czasami tej zmiany nie b�dzie dla kolejnego ruchu (wielokrotne bicie)
};


moveID GetAImove(Board);  // funkcja zwracaj�ca ruch wybrany przez algorytm sztucznej inteligencji
int heuristicForCheckers(Board, bool); // funckja heurystyczna obliczajaca warto�� danego w�z�a
int minmax(TreeNode tree);  // algorytm sztucznej inteligencji - min
