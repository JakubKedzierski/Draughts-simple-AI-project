#include "checkersAI.hpp"

using namespace std;

moveID checkersAI::GetAIindexToMove(const Board board) {
	
	tempBoard = board;
	TreeNode tree(4,tempBoard);
	moveID move(0, 0, 0, 0);
	int choice=-Infty;
	
	
	int tmp;
	for (auto son : tree.sons) {
		tmp = minmax(son);
		if (tmp >= choice) {
			choice = tmp;
			move = son.move;
		}
	}
	

	return move;
}

int minmax(TreeNode tree) {
	
	TreeNode ret;
	int choice=0;


	if (tree.Player) {
		choice = -Infty;
	}
	else {
		choice = Infty;
	}
	

	for (auto son : tree.sons) {
		
		if (son.Player) {
			choice = min(son.heuristicGrade, choice);
			ret = son;
		}

		if (!son.Player) {
			choice = max(son.heuristicGrade, choice);
			ret = son;
		}
	}

	if (tree.sons.size()) {
		if (tree.sons[0].depth > 0) {
			choice += minmax(ret);
		}
	}

	return choice;
}

void TreeNode::MakeTree() {
	vector<moveID> moves;
	bool moreBeats=false;
	
	moves = boardStatus.CheckForBeatings(Player);
	if (moves.size() > 0) {
		moreBeats = true;
	}
	else {
		moreBeats = false;
		moves = boardStatus.PossibleMove(Player);
	}

	if (moves.size() == 0 && Player) { heuristicGrade = -Infty; return; }
	else if (moves.size() == 0) { heuristicGrade = Infty; return;}

	
	for (auto &choice : moves) {
		TreeNode temp;
		temp = *this;         // tu zastapic to this 
		temp.sons.clear();
		temp.boardStatus.Move(choice);
		temp.depth--;
		temp.move = choice;
		temp.heuristicGrade = heuristicForCheckers(temp.boardStatus, temp.Player);
		
		temp.CheckForPlayer(move);
		temp.FewBeatings = moreBeats;
		sons.push_back(temp);
		

		if (temp.depth > 0) {
			sons.back().MakeTree();
		}

	}
	moves.clear();


	
	// to po ruchu if  (!(FewBeatings && son.CheckForBeatings(data.Player).size() > 0))  Player = !Player;

}

void TreeNode::CheckForPlayer(moveID prevMove) {
	vector<moveID> Possible;
	Possible = boardStatus.CheckForBeatings(Player);

	for (auto newBeat : Possible) {
		if (prevMove.y1 == newBeat.y && prevMove.x1 == newBeat.x) {
			if (FewBeatings) {
				return;
			}
		}
	}

	Player = !Player;
}





// pionek = 50
// damka =100
// centrum =8
// slabsze centrum = 5
int heuristicForCheckers(Board board, bool Player) {
	int value = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			
			if (board(i,j)!=Empty) {

				switch (board(i, j)) {

				case WhiteMan: {
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value -= 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value -= 5;
					value -= 50;
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
					if ((i > 1 && i < 6) && (j > 1 && j < 6)) value += 3;
					if ((i > 0 && i < 7) && (j > 0 && j < 7)) value += 5;
					value += 50;
					break;
				}
				
				}
			}

		}
	}
	return value;
}