#include <iostream>
#include <queue>
#define BLOCKED 100
using namespace std;

int n, m;
int board[16][16];
pair<int, int> conv[31];
pair<int, int> player[31];
bool isArrive[31];
int playTime;
int drow[] = { -1, 0, 0, 1 };
int dcol[] = { 0, -1, 1, 0 };
void initValue()
{
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cin >> board[i][j];
		}
	}
	for (int i = 1; i <= m; i++)
	{
		int row, col;
		cin >> row >> col;
		conv[i] = make_pair(row, col);
		board[row][col] = -i;
		isArrive[i] = false;
	}
}
void moveToConv()
{
	for (int i = 1; i < playTime && i <= m; i++)
	{
		if (!isArrive[i])
		{
			pair<int, int> convPos = conv[i];
			queue<pair<int, int> > q;
			pair<int, int> ch[16][16];
			int firstMove[16][16];
			for (int a = 1; a <= n; a++)
			{
				for (int b = 1; b <= n; b++)
				{
					ch[a][b] = make_pair(-1, -1);
					firstMove[a][b] = -1;
				}
			}
			q.push({ player[i].first, player[i].second });
			ch[player[i].first][player[i].second] = { player[i].first, player[i].second };
			while (!q.empty())
			{
				pair<int, int> curPos = q.front();
				q.pop();
				if (curPos == convPos)
				{
					break;
				}
				for (int j = 0; j < 4; j++)
				{
					int nRow = curPos.first + drow[j];
					int nCol = curPos.second + dcol[j];
					if (nRow > n || nRow < 1 || nCol > n || nCol < 1) continue;
					if (ch[nRow][nCol] == make_pair(-1, -1) && board[nRow][nCol] != BLOCKED)
					{
						if (firstMove[curPos.first][curPos.second] == -1) firstMove[nRow][nCol] = j;
						else
							firstMove[nRow][nCol] = firstMove[curPos.first][curPos.second];

						q.push({ nRow, nCol });
						ch[nRow][nCol] = curPos;
					}
				}
			}
			player[i].first += drow[firstMove[convPos.first][convPos.second]];
			player[i].second += dcol[firstMove[convPos.first][convPos.second]];
			if (player[i] == conv[i]) isArrive[i] = true;
		}
	}
}
void blockBoard()
{
	for (int i = 1; i <= m; i++)
	{
		if (isArrive[i] == true)
		{
			board[conv[i].first][conv[i].second] = BLOCKED;
		}
	}
}
void newPlayerIn()
{
	// bfs를 통해 baseCamp탐색
	if (playTime <= m)
	{
		int r, c;
		r = c = 0;
		pair<int, int> convPos = conv[playTime];
		queue<pair<int, int> > q;
		int ch[16][16] = { 0 };
		q.push(convPos);
		ch[convPos.first][convPos.second] = 1;
		bool isFound = false;
		int minDist = -1;
		while (!q.empty())
		{
			pair<int, int> pos = q.front();
			q.pop();
			for (int i = 0; i < 4; i++)
			{
				int nRow = pos.first + drow[i];
				int nCol = pos.second + dcol[i];
				if (nRow > n || nRow < 1 || nCol >n || nCol < 1) continue;
				if (ch[nRow][nCol] == 0 && board[nRow][nCol] != BLOCKED)
				{
					ch[nRow][nCol] = ch[pos.first][pos.second] + 1;
					q.push({ nRow, nCol });
					if (board[nRow][nCol] == 1 && !isFound)
					{
						minDist = ch[nRow][nCol];
						isFound = true;
					}
				}
			}
		}

		bool isSelected = false;
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				if (board[i][j] == 1 && ch[i][j] == minDist && !isSelected)
				{
					r = i;
					c = j;
					isSelected = true;
				}
			}
		}
		// 선택 완료!
		board[r][c] = BLOCKED;
		player[playTime] = make_pair(r, c);
	}
}
bool isEndGame()
{
	bool isEnd = true;
	for (int i = 1; i <= m; i++)
	{
		if (!isArrive[i])
		{
			isEnd = false;
			break;
		}
	}
	return isEnd;
}
int main()
{
	initValue();
	playTime = 1;
	while (1)
	{
		moveToConv();
		blockBoard();
		newPlayerIn();
		// 모든 액션완료
		// 종료되지 않았으면 시간증가
		if (!isEndGame())
			playTime++;
		else
			break;
	}
	cout << playTime;
}