#include "checkersAI.hpp"

using namespace std;

moveID checkersAI::GetAIindexToMove(const Board board) {
	
	tempBoard = board;
	TreeNode tree(2,tempBoard);
	

	moveID move = minmax(tree);

	return move;
}

moveID checkersAI::minmax(TreeNode tree) {
	moveID move;

	int choice = -10000, iter = 0; 
	int max2 = 0;

	for (int i = 0; i < tree.GetSons().size(); i++) {
		for(int j=0;j<tree.GetSons()[i].GetSons().size();j++){
			if (tree.GetSons()[i].GetSons()[j].heuristicGrade > max2) {
				max2 = tree.GetSons()[i].heuristicGrade;
				iter = j;
			}
		}
		if (tree.GetSons()[i].heuristicGrade - max2 > choice) {
			choice = tree.GetSons()[i].heuristicGrade - max2;
			move = tree.GetSons()[i].move;
		}
		max2 = 0;
	}

	return move;
}

void TreeNode::MakeTree() {
	vector<moveID> moves;
	//for
	
	moves = boardStatus.CheckForBeatings(Player);
	if (moves.size() > 0) {
		FewBeatings = true;
	}
	else {
		FewBeatings = false;
		moves = boardStatus.PossibleMove(Player);
	}

	
	for (auto &choice : moves) {
		TreeNode temp;
		temp = *this;
		temp.sons.clear();
		temp.boardStatus.Move(choice);
		temp.depth--;
		temp.Player = !temp.Player;
		temp.move = choice;
		temp.heuristicGrade = heuristicForCheckers(temp.boardStatus, temp.Player);
		sons.push_back(temp);
		
		if (temp.depth > 0) {
			sons.back().MakeTree();
		}
	}
	moves.clear();
	
	// to po ruchu if  (!(FewBeatings && son.CheckForBeatings(data.Player).size() > 0))  Player = !Player;

}

// pionek = 20
// damka =50
// centrum =8
// slabsze centrum = 5
int heuristicForCheckers(Board board, bool Player) {
	int value = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			
			if (Player && board(i,j)!=Empty) {

				if ((i > 1 && i < 6) && (j > 1 && j < 6)) value += 3;
				if ((i > 0 && i < 7) && (j > 0 && j < 7)) value += 5;

				switch (board(i, j)) {

				case WhiteMan: {
					value += 20;
					break;
				}

				case WhiteKing: {
					value += 50;
					break;
				}
				}
			}
			else if(!Player && board(i, j) != Empty){

				if ((i > 1 && i < 6) && (j > 1 && j < 6)) value += 3;
				if ((i > 0 && i < 7) && (j > 0 && j < 7)) value += 5;
				
				switch (board(i, j)) {
				case BlackKing: {
					value += 50;
					break;
				}

				case BlackMan: {
					value += 20;
					break;
				}
				
				}
			}

		}
	}
	return value;
}