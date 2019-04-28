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
	pos selectMaxWin1(board &tempBoard)
	{
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
		for (auto it = validPosition.begin(); it != validPosition.end(); it++)
		{
			if(winNum[i] >= best)
			{
				bestPos = *it;
				best = winNum[i++];
			}
		}
		return bestPos;
	}

	// MCTS+UCB
	pos selectMaxWin2(board &tempBoard)
	{
		set<pos> validPosition;
		board C(tempBoard);
		const int currentComposition = C.getComposition(validPosition);
		assert(currentComposition == -1);	// continue game
		int tmpPlayer = tempBoard.curPlayer;
		
			//the total number of winnings
			int moves[64] = { 0 };
			//the total number of simulations for the node
			int N[64] = { 0 };
			double statisticsConfidenceIntervals[64] = { 0 };
			int sz = validPosition.size();
			// the total number of simulations
			int cnt = 0;
			//the total number of plays
			const auto beginTime = static_cast<clock_t>(time(nullptr));
			int curTime = static_cast<clock_t>(time(nullptr));
			for (const auto& it : validPosition)
			{
				tmpPlayer = tempBoard.curPlayer;
				const int save = cnt % sz;
				cnt++;
				board tmp(tempBoard);
				tmp.placeOn(it, tmp.curPlayer);
				int res = -1;
				simulate(tmp, res);
				moves[save] += res;
				N[save]++;
			}

			while (curTime < beginTime + t_sim / 1000) {
				int iteration = 0;
				for (auto it = validPosition.begin(); it != validPosition.end(); it++) {
					iteration++;
					int largest = 0;
					pos largest_pos = 0;
					int i = 0;
					for (const auto& it_in : validPosition)
					{
						i++;
						statisticsConfidenceIntervals[i] = static_cast<double>(moves[i]) / cnt + sqrt(log(N[i]) / cnt);
						if (statisticsConfidenceIntervals[i] > largest) {
							largest_pos = it_in;
							largest = statisticsConfidenceIntervals[i];
						}
					}
					N[largest]++;
					int save = cnt % sz;//the sequence num of the machine
					cnt++;

					tmpPlayer = tempBoard.curPlayer;
					board tmp(tempBoard);
					tmp.placeOn(largest_pos, tmp.curPlayer);
					int res = -1;
					simulate(tmp, res);
					moves[largest] += res;
					statisticsConfidenceIntervals[largest] = static_cast<double>(moves[largest]) / cnt + sqrt(log(N[largest]) / cnt);

				}
				curTime = static_cast<clock_t>(time(nullptr));
			}
			int greatest = 0;
			pos greatestPos;
			//choose the greatest statistics confidence intervals from all
			int i = 0;
			for (auto it = validPosition.begin(); it != validPosition.end(); it++) {
				if (statisticsConfidenceIntervals[i++] >= greatest) {
					greatestPos = *it;
					greatest = moves[i - 1];
				}
			}
			return greatestPos;
	}
};