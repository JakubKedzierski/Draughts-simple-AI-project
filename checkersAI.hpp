#pragma once
#include "board.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;

struct NodeData {
	int heuristicGrade;
	Board boardStatus;
	moveID move;
	bool Player;
};

class TreeNode {
	int depth;  // liczba krokow liczona do przodu
	NodeData data;
	vector<TreeNode> sons;
	bool FewBeatings;
public:
	TreeNode(){}
	TreeNode(int steps, Board b) { data.boardStatus = b; depth = steps; data.Player = false; FewBeatings = false; MakeTree(); }
	void MakeTree();
};



class checkersAI {
	Board tempBoard;
public:
	checkersAI() :tempBoard() {}
	int GetAIindexToMove(Board);
};