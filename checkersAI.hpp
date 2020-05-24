#pragma once
#include "board.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
const int Infty = 9999999;

int heuristicForCheckers(Board, bool);


class TreeNode {
public:
	int heuristicGrade;
	Board boardStatus;
	moveID move;
	bool Player;
	int depth;  // liczba krokow liczona do przodu
	vector<TreeNode> sons;
	bool FewBeatings;

	TreeNode() {}
	TreeNode(int steps, Board b) {
		boardStatus = b; depth = steps; Player = false; FewBeatings = false; 
		MakeTree();
	}
	void MakeTree();
	vector<TreeNode> GetSons() { return sons; }
	int Depth() { return depth; }
	void CheckForPlayer(moveID prevMove);
};



class checkersAI {
	Board tempBoard;
public:
	checkersAI() :tempBoard() {}
	moveID GetAIindexToMove(Board);
};


int minmax(TreeNode tree);
