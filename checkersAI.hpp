#pragma once
#include "board.hpp"
#include <vector>
const long int Infty = 9999999;

// Wierzcholki drzewa mo¿liwych rozwi¹zañ gry (ruchów do przodu)
class TreeNode {
public:
	/* Informacje przechowywane w wierzcho³ku drzewa : */

	int heuristicGrade;  // wartoœæ danego wierzcho³ka obliczona przez funkcje heurystyczna / ocena danego ruchu
	Board boardStatus;   // status na planszy
	moveID move;		 // ruch który jest analizowany w danym wierzcho³ku
	bool Player;		 // gracz który rusza siê w danym wierzcho³ku
	int depth;			 // liczba krokow liczona do przodu od danego wierzcho³ka
	vector<TreeNode> sons;  // "synowie" wierzcho³ka - mo¿liwe rozwi¹zania gry po wykonaniu ruchu w tym wierzcho³ku
	bool FewBeatings;	 // zmienna do zapamiêtywania ewentualnego wielokrotnego bicia
	
	/***************************************************/
	
	TreeNode() {}

	/* Tworzenie drzewa mo¿liwych rozwi¹zañ */
	TreeNode(int steps, Board b) {
		boardStatus = b; depth = steps; Player = false; FewBeatings = false;  
		MakeSons();
	}
	
	void MakeSons(); // Tworzenie "synów " czyli kolejnych kroków, mo¿liwych rozwi¹zañ gry
	void CheckForPlayer(moveID prevMove); // Zmiana gracza (czasami tej zmiany nie bêdzie dla kolejnego ruchu (wielokrotne bicie)
};


moveID GetAImove(Board);  // funkcja zwracaj¹ca ruch wybrany przez algorytm sztucznej inteligencji
int heuristicForCheckers(Board, bool); // funckja heurystyczna obliczajaca wartoœæ danego wêz³a
int minmax(TreeNode tree);  // algorytm sztucznej inteligencji - min
