#include <iostream>
#include <set>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>
#include "pos.h"
#include "board.h"

const clock_t t_sim = 2 * CLOCKS_PER_SEC;

class mcts
{
public:
	mcts() {};

	pos inputPosition() {
		int x, y;
		cout << "Please give your move. [Format: x y]" << endl;
		cin >> x >> y;
		pos p(x, y);
		return p;
	}



	void simulate1(board &tempBoard, int &result) {
		set<pos> tempNodes;
		const int tempComposition = tempBoard.getComposition(tempNodes);
		switch (tempComposition) {
		case -1: {
			board tBoard(tempBoard);    // copy current composition
			const int treeSize = tempNodes.size();
			srand(static_cast<unsigned int>(time(nullptr)));
			const int num = rand() % treeSize;
			auto iter = tempNodes.begin();
			advance(iter, num);
			tBoard.placeOn(*iter, tBoard.curPlayer);
			return simulate1(tBoard, result);
		}
		case 2:
			result = 1;
			break;
		default:
			result = 0;
			break;
		}
		return;
	}

	// MCTS+random
	pos selectMaxWin1(board &tempBoard) {
		set<pos> validPosition;
		board tempBoard_ucb(tempBoard);
		int ucbComposition = tempBoard_ucb.getComposition(validPosition);
		int tPlayer = tempBoard.curPlayer;
		assert(ucbComposition == -1);	// continue game
		int winNum[64] = { 0 };
		int simTimes[64] = { 0 };

	}

	// MCTS+UCB
	void selectMaxWin2(board &tempBoard, int ai) {

	}
};