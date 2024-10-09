#include <iostream>
#include <queue>
#include <vector>
#include <tuple>
#define ROO -1
using namespace std;
int mapSize, totalTurn, totalSanta, rooPower, santaPower;
int turn;
pair<int, int> roo;
pair<int, int> santa[31];
int stun[31];
int score[31];
bool die[31];
int board[51][51];
int drow[] = { -1, 0, 1, 0 };
int dcol[] = { 0, 1, 0, -1 };
void getInput()
{
	cin >> mapSize >> totalTurn >> totalSanta >> rooPower >> santaPower;
	cin >> roo.first >> roo.second;
	for (int i = 1; i <= totalSanta; i++)
	{
		int santaNo, row, col;
		cin >> santaNo >> row >> col;
		santa[santaNo] = make_pair(row, col);
		board[row][col] = santaNo;
	}
}
void printScore()
{
	for (int i = 1; i <= totalSanta; i++)
		cout << score[i] << " ";
}
int getCloseSanta()
{
	int minDist = 2147000000;
	int santaIdx = 0;
	for (int i = 1; i <= totalSanta; i++)
	{
		if (!die[i])
		{
			int rDist = (roo.first - santa[i].first) * (roo.first - santa[i].first);
			int cDist = (roo.second - santa[i].second) * (roo.second - santa[i].second);
			int dist = rDist + cDist;
			if (minDist > dist)
			{
				minDist = dist;
				santaIdx = i;
			}
			else if (minDist == dist)
			{
				if (santa[santaIdx].first < santa[i].first)
					santaIdx = i;
				else if (santa[santaIdx].first == santa[i].first)
				{
					if (santa[santaIdx].second < santa[i].second)
						santaIdx = i;
				}
			}
		}
	}
	return santaIdx;
}
void kickSanta(int power, int drow, int dcol, int row, int col)
{
	vector<tuple<int, int, int>> targets;
	int idx = board[row][col];
	stun[idx] = 2;
	score[idx] += power;
	int nRow = row + drow * power;
	int nCol = col + dcol * power;
	targets.push_back(make_tuple(idx, nRow, nCol));
	// 루돌프와 만난 산타가 간곳이 맵 밖임 -> 사망
	if (nRow > mapSize || nRow < 1 || nCol > mapSize || nCol < 1) die[idx] = true;
	// 아님!
	else
	{
		while (1)
		{
			// 아무도 안만남
			if (board[nRow][nCol] == 0)
			{
				break;
			}
			// 다른 산타와 만남
			else if (board[nRow][nCol] != 0)
			{
				// 해당 산타를 이제 검사대상으로 설정
				idx = board[nRow][nCol];
				// 해당 산타가 밀쳐질 곳
				nRow += drow;
				nCol += dcol;
				targets.push_back(make_tuple(idx, nRow, nCol));
				// 맵밖이면 사망 연쇄 끝
				if (nRow > mapSize || nRow < 1 || nCol > mapSize || nCol < 1)
				{
					die[idx] = true;
					break;
				}
			}
		}
	}
	for (int i = targets.size() - 1; i >= 0; i--)
	{
		int sIdx, nextRow, nextCol;
		tie(sIdx, nextRow, nextCol) = targets[i];
		if (!die[sIdx])
		{
			board[santa[sIdx].first][santa[sIdx].second] = 0;
			santa[sIdx] = make_pair(nextRow, nextCol);
			board[santa[sIdx].first][santa[sIdx].second] = sIdx;
		}
		else if (die[sIdx])
		{
			board[santa[sIdx].first][santa[sIdx].second] = 0;
		}
	}

}
void rooPlay()
{
	int santaIdx = getCloseSanta();
	// 가장가까운 산타로 이동
	int nrow, ncol;
	nrow = ncol = 0;
	if (roo.first < santa[santaIdx].first) nrow = 1;
	if (roo.first > santa[santaIdx].first) nrow = -1;
	if (roo.second < santa[santaIdx].second) ncol = 1;
	if (roo.second > santa[santaIdx].second) ncol = -1;
	roo.first += nrow;
	roo.second += ncol;
	// 산타와 만나면 충돌 발생
	if (board[roo.first][roo.second] != 0)
	{
		kickSanta(rooPower, nrow, ncol, roo.first, roo.second);
	}
}
void sanPlay()
{
	for (int i = 1; i <= totalSanta; i++)
	{
		if (!die[i] && stun[i] == 0)
		{
			//pair<int, int> prevPos = santa[i];
			int rDist = (roo.first - santa[i].first) * (roo.first - santa[i].first);
			int cDist = (roo.second - santa[i].second) * (roo.second - santa[i].second);
			int dist = rDist + cDist;
			bool isAble = false;
			int dir = 0;
			for (int j = 0; j < 4; j++)
			{
				int nRow = santa[i].first + drow[j];
				int nCol = santa[i].second + dcol[j];
				if (nRow > mapSize || nRow < 1 || nCol > mapSize || nCol < 1) continue;
				if (board[nRow][nCol] != 0) continue;
				int newrDist = (roo.first - nRow) * (roo.first - nRow);
				int newcDist = (roo.second - nCol) * (roo.second - nCol);
				int newDist = newrDist + newcDist;
				if (newDist < dist)
				{
					isAble = true;
					dist = newDist;
					dir = j;
				}
			}
			if (isAble)
			{
				board[santa[i].first][santa[i].second] = 0;
				santa[i] = make_pair(santa[i].first + drow[dir], santa[i].second + dcol[dir]);
				board[santa[i].first][santa[i].second] = i;

				if (santa[i] == roo)
				{
					kickSanta(santaPower, -drow[dir], -dcol[dir], santa[i].first, santa[i].second);
				}
			}
		}
	}
}
void endTurn()
{
	for (int i = 1; i <= totalSanta; i++)
	{
		if (!die[i]) score[i]++;
		if (stun[i] > 0) stun[i]--;
	}
}
bool isEndGame()
{
	for (int i = 1; i <= totalSanta; i++)
		if (!die[i]) return false;
	return true;
}
int main()
{
	getInput();
	for (turn = 1; turn <= totalTurn; turn++)
	{
		if (isEndGame()) break;
		rooPlay();
		sanPlay();
		endTurn();
	}
	printScore();
}