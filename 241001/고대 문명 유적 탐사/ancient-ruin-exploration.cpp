#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct InitInfo
{
	int row;
	int col;
	int rotateCnt;
	InitInfo(int _row, int _col, int _rotateCnt)
		: row(_row), col(_col), rotateCnt(_rotateCnt) {}
	bool operator<(const InitInfo& target) const
	{
		if (this->rotateCnt != target.rotateCnt) 
			return this->rotateCnt < target.rotateCnt;
		else if (this->col != target.col)
			return this->col < target.col;
		else if (this->row != target.row) 
			return this->row < target.row;
	}
};
int k, m, res, tempRes, initRes;
InitInfo info(0, 0, 0);
int cRow[] = { 1, 1, 1, 2, 2, 2, 3, 3, 3 };
int cCol[] = { 1, 2, 3, 1, 2, 3, 1, 2, 3 };
int dRow[] = { 1, 0, -1, 0 };
int dCol[] = { 0, -1, 0, 1 };
queue<int> piece;
vector<vector<int>> bfs(vector<vector<int>> board)
{
	tempRes = 0;
	vector<vector<int>> ch(5, vector<int>(5));
	queue<pair<int, int>> q;
	int caseNo = 1;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (ch[i][j] == 0)
			{
				caseNo++;
				q.push({ i, j });
				ch[i][j] = caseNo;
				int cnt = 1;
				while (!q.empty())
				{
					pair<int, int> pos = q.front();
					q.pop();
					for (int k = 0; k < 4; k++)
					{
						int nRow = pos.first + dRow[k];
						int nCol = pos.second + dCol[k];
						if (nRow < 5 && nRow >= 0 &&
							nCol < 5 && nCol >= 0 &&
							ch[nRow][nCol] == 0 &&
							board[nRow][nCol] == board[pos.first][pos.second])
						{
							ch[nRow][nCol] = caseNo;
							cnt++;
							q.push({ nRow, nCol });
						}
					}
				}
				if (cnt >= 3)
				{
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 5; j++)
						{
							if (ch[i][j] == caseNo)
							{
								board[i][j] = 0;
							}
						}
					}
					tempRes += cnt;
				}
			}
		}
	}
	return board;
}
void fillIn(vector<vector<int>>& board, queue<int>& piece)
{
	for (int j = 0; j <= 4; j++)
	{
		for (int i = 4; i >= 0; i--)
		{
			if (board[i][j] == 0)
			{
				board[i][j] = piece.front();
				piece.pop();
			}
		}
	}
}
vector<vector<int>> rotate(vector<vector<int>> board, InitInfo& info)
{
	for (int i = 1; i <= info.rotateCnt; i++)
	{
		int temp = board[info.row - 1][info.col - 1];
		board[info.row - 1][info.col - 1] = board[info.row + 1][info.col - 1];
		board[info.row + 1][info.col - 1] = board[info.row + 1][info.col + 1];
		board[info.row + 1][info.col + 1] = board[info.row - 1][info.col + 1];
		board[info.row - 1][info.col + 1] = temp;
		temp = board[info.row - 1][info.col];
		board[info.row - 1][info.col] = board[info.row][info.col - 1];
		board[info.row][info.col - 1] = board[info.row + 1][info.col];
		board[info.row + 1][info.col] = board[info.row][info.col + 1];
		board[info.row][info.col + 1] = temp;
	}
	return board;
}
int main()
{
	vector<vector<int>> board(5, vector<int>(5));
	cin >> k >> m;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			cin >> board[i][j];
	int temp;
	for (int i = 0; i < m; i++)
	{
		cin >> temp;
		piece.push(temp);
	}
	for (int i = 0; i < k; i++)
	{
		initRes = 0;
		res = 0;
		// 탐사 시작
		for (int j = 0; j < 9; j++)
		{
			for (int k = 1; k <= 3; k++)
			{
				InitInfo curInfo(cRow[j], cCol[j], k);
				bfs(rotate(board, curInfo));
				if (tempRes > initRes)
				{
					info = curInfo;
					initRes = tempRes;
				}
				else if (tempRes == initRes)
				{
					if (curInfo < info)
						info = curInfo;
				}
			}
		}
		// 탐사 끝
		if (initRes == 0)
			break;
		else
		{
			board = bfs(rotate(board, info));
			res += tempRes;
			fillIn(board, piece);
			while (1)
			{
				board = bfs(board);
				if (tempRes == 0)
					break;
				else
				{
					res += tempRes;
					fillIn(board, piece);
				}
			}
		}
		cout << res << ' ';
	}
}