#include "checkersAI.hpp"

using namespace std;

int checkersAI::GetAIindexToMove(const Board board) {
	
	tempBoard = board;
	TreeNode tree(2,tempBoard);

	return 0;
}

void TreeNode::MakeTree() {
	vector<moveID> moves;
	//for
	moves = data.boardStatus.CheckForBeatings(data.Player);
	if (moves.size() > 0) {
		FewBeatings = true;
	}
	else {
		FewBeatings = false;
		moves = data.boardStatus.PossibleMove(data.Player);
	}
	
	TreeNode temp;
	for (auto choice : moves) {
		temp = *this;
		temp.data.boardStatus.Move(choice);
		temp.depth--;
		temp.data.Player = !temp.data.Player;
		temp.data.move = choice;
		
		sons.push_back(temp);
	}


	if (temp.depth > 1) {
		for (auto son : sons) {
			son.MakeTree();
			temp.data.boardStatus.PrintToStream();
		}
	}
	
	// to po ruchu if  (!(FewBeatings && son.CheckForBeatings(data.Player).size() > 0))  Player = !Player;

}