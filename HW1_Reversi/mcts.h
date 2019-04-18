#include <iostream>
#include <set>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>
#include "pos.h"
#include "board.h"

const clock_t t_sim = 3 * CLOCKS_PER_SEC;

class mcts
{
public:
	mcts() {};

	void simulate(board &tempBoard, int &result) {
		set<pos> tempNodes;
		const int tempComposition = tempBoard.getComposition(tempNodes);
		switch (tempComposition) {
		case -1: {
			board tBoard(tempBoard);    // copy current composition
			const int treeSize = tempNodes.size();
			srand(static_cast<unsigned int>(time(nullptr)));
			const int num = rand() % treeSize;
			auto it = tempNodes.begin();
			advance(it, num);
			tBoard.placeOn(*it, tBoard.curPlayer);
			return simulate(tBoard, result);
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
		board tBoard(tempBoard);
		const int currentComposition = tBoard.getComposition(validPosition);
		assert(currentComposition == -1);	// continue game
		int winNum[64] = { 0 };
		const int nodeNum = validPosition.size();
		int c1 = 0;
		int c2 = 0;
		const auto start = static_cast<clock_t>(time(nullptr));
		int tempTime = static_cast<clock_t>(time(nullptr));
		while(tempTime < start + t_sim / 1000)
		{
			for(auto it = validPosition.begin(); it != validPosition.end(); ++it)
			{
				const int save = c1 % nodeNum;
				c1++;
				board t(tempBoard);
				t.placeOn(*it, t.curPlayer);
				int res = -1;
				simulate(t, res);
				winNum[save] = winNum[save] + res;
			}
			tempTime = static_cast<clock_t>(time(nullptr));
		}
		int best = 0;
		int i = 0;
		pos bestPos;
		for (auto it = validPosition.begin(); it != validPosition.end(); ++it)
		{
			if(winNum[i] > best)
			{
				bestPos = *it;
				best = winNum[i++];
			}
		}
		return bestPos;
	}

	// MCTS+UCB
	void selectMaxWin2(board &tempBoard, int ai) {

	}
};