#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int drow[] = { -1, 1, 0, 0 };
int dcol[] = { 0, 0, 1, -1 };
struct Player {
	int dist = 0;
	int r = 0;
	int c = 0;
	bool isDone = false;
};

struct Goal {
	int r = 0;
	int c = 0;
};

struct Rect {
	int r = 0;
	int c = 0;
	int lineLength = 0;
	Rect(int _r, int _c, int _l)
	{
		r = _r;
		c = _c;
		lineLength = _l;
	}
};
int n, m, k;
int board[11][11];
Player players[11];
Goal goal;
void getInput()
{
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
			cin >> board[i][j];
	}
	for (int i = 1; i <= m; i++)
	{
		int r, c;
		cin >> r >> c;
		players[i].r = r;
		players[i].c = c;
	}
	int r, c;
	cin >> r >> c;
	goal.r = r;
	goal.c = c;
}
bool isEnd()
{
	for (int i = 1; i <= m; i++)
	{
		if (!players[i].isDone) return false;
	}
	return true;
}
void printRes()
{
	int res = 0;
	for (int i = 1; i <= m; i++)
	{
		res += players[i].dist;
	}
	cout << res << '\n';
	cout << goal.r << ' ' << goal.c;
}
void playGame();
void movePlayer();
void rotateBoard();
Rect getRect();
void rotate(int rr, int cc, int len);
int calDist(pair<int, int> pos1, pair<int, int> pos2);
int main()
{
	getInput();
	for (int i = 1; i <= k; i++)
	{
		//cout << "게임순서"<<i << '\n';
		if (!isEnd())
			playGame();
		//cout << '\n';
		//for (int a = 1; a <= n; a++)
		//{
		//	for (int b = 1; b <= n; b++)
		//	{
		//		cout << board[a][b] << '\t';
		//	}
		//	cout << '\n';
		//}
		//for (int a = 1; a <= m; a++)
		//	cout << "player" << a << " " << players[a].r << " " << players[a].c << '\n';
	}
	printRes();
}

void playGame()
{
	movePlayer();
	if (!isEnd())
		rotateBoard();
}

void movePlayer()
{
	for (int idx = 1; idx <= m; idx++)
	{
		if (players[idx].isDone) continue;
		pair<int, int> curPos = { players[idx].r , players[idx].c };
		pair<int, int> goalPos = { goal.r , goal.c };
		int minDist = calDist(curPos, goalPos);
		bool isMove = false;
		int dir = -1;
		for (int i = 0; i < 4; i++)
		{
			int nRow = curPos.first + drow[i];
			int nCol = curPos.second + dcol[i];
			if (nRow <= n && nRow >= 1 &&
				nCol <= n && nCol >= 1 &&
				board[nRow][nCol] == 0)
			{
				if (minDist > calDist({ nRow, nCol }, goalPos))
				{
					isMove = true;
					minDist = calDist({ nRow, nCol }, goalPos);
					dir = i;
				}
			}
		}
		if (dir != -1)
		{
			players[idx].r += drow[dir];
			players[idx].c += dcol[dir];
			players[idx].dist += 1;
			if (players[idx].r == goal.r &&
				players[idx].c == goal.c)
			{
				players[idx].isDone = true;
				//cout << idx << "haha out !!\n";
			}
		}
	}
}
int calDist(pair<int, int> pos1, pair<int, int> pos2)
{
	return abs(pos1.first - pos2.first) + abs(pos1.second - pos2.second);
}
void rotateBoard()
{
	// 사각형 찾기
	Rect rect = getRect();
	// 회전
	rotate(rect.r, rect.c, rect.lineLength);
}
Rect getRect()
{
	for (int i = 2; i <= n; i++)
	{
		for (int a = goal.r - i + 1; a <= goal.r; a++)
		{
			//cout << a <<' ';
			for (int b = goal.c - i + 1; b <= goal.c; b++)
			{
				//cout << b <<'\n';
				if (a < 1 || b < 1 || b > n) continue;
				for (int idx = 1; idx <= m; idx++)
				{
					if (players[idx].r >= a && players[idx].r <= a + i - 1 &&
						players[idx].c >= b && players[idx].c <= b + i - 1 &&
						!players[idx].isDone)
					{
						//cout << "found rect: " << a << " " << b << " " << i << '\n';
						return Rect(a, b, i);
					}
				}
			}
		}
	}
}

void rotate(int rr, int cc, int len)
{
	int t[11][11] = { 0 };
	// Decrease values in the specified subarray
	for (int i = rr; i < rr + len; i++)
	{
		for (int j = cc; j < cc + len; j++)
		{
			if (board[i][j] > 0) board[i][j]--;
		}
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
			t[i][j] = board[i][j];
	}

	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			board[rr + j][cc + len - 1 - i] = t[rr + i][cc + j];
		}
	}
	for (int i = 1; i <= m; i++)
	{
		if (players[i].r >= rr && players[i].r <= rr + len  - 1&&
			players[i].c >= cc && players[i].c <= cc + len - 1 &&
			!players[i].isDone)
		{
			//cout << i << " is in!!\n";
			int new_r = rr + (players[i].c - cc);
			int new_c = cc + len - 1 - (players[i].r - rr);
			players[i].r = new_r;
			players[i].c = new_c;
		}
	}
	int new_r = rr + (goal.c - cc);
	int new_c = cc + len - 1 - (goal.r - rr);
	goal.r = new_r;
	goal.c = new_c;
}