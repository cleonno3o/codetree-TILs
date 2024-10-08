#include <iostream>
#include <queue>
using namespace std;

struct Team {
	pair<int, int> head;
	pair<int, int> tail;
	Team() = default;
	Team(pair<int, int> _head, pair<int, int> _tail)
		: head(_head), tail(_tail) {}
};
int board[21][21];
int teamBoard[21][21];
Team teams[6];
int n, m, k;
int score, turn;
int drow[] = { 1, 0, -1, 0 };
int dcol[] = { 0, -1, 0, 1 };
int sdrow[] = { 0, -1, 0, 1 };
int sdcol[] = { 1, 0, -1, 0 };
int dir = 0;
int offset = 1;
void getInput()
{
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cin >> board[i][j];
		}
	}
}
void getTeamPathBfs(int row, int col, int teamIdx)
{
	queue<pair<int, int> > q;
	q.push({ row, col });
	teamBoard[row][col] = teamIdx;
	// 1은 머리이므로 대입
	teams[teamIdx].head = { row, col };
	while (!q.empty())
	{
		pair<int, int> p = q.front();
		q.pop();
		// 3번이면 꼬리이므로 대입
		if (board[p.first][p.second] == 3) teams[teamIdx].tail = p;
		for (int i = 0; i < 4; i++)
		{
			int nRow = p.first + drow[i];
			int nCol = p.second + dcol[i];
			if (nRow > n || nRow < 1 || nCol > n || nCol < 1) continue;
			if (board[nRow][nCol] == 0) continue;
			if (teamBoard[nRow][nCol] == 0)
			{
				teamBoard[nRow][nCol] = teamIdx;
				q.push({ nRow, nCol });
			}
		}
	}
}
void initTeam()
{
	int foundTeam = 0;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			// 팀의 시작점 발견
			if (board[i][j] == 1)
			{
				foundTeam++;
				getTeamPathBfs(i, j, foundTeam);
			}
		}
	}
}
void moveTeam()
{
	// 모든 팀에 대해 한 칸 이동
	for (int i = 1; i <= m; i++)
	{
		//cout << i << " 팀\n";
		// 1에서 가야할곳 찾고
		for (int j = 0; j < 4; j++)
		{
			int nRow = teams[i].head.first + drow[j];
			int nCol = teams[i].head.second + dcol[j];
			if (nRow > n || nRow < 1 || nCol > n || nCol < 1) continue;
			// 내 4번인지 확인
			if (board[nRow][nCol] == 4)
			{
				//cout << nRow << " " << nCol << "이 4임을발견\n";
				// 4로 이동후
				board[nRow][nCol] = 1;
				// 기존 1칸을 2로 채움
				//cout << teams[i].head.first << " " << teams[i].head.second << "를 2로 변경\n";
				board[teams[i].head.first][teams[i].head.second] = 2;
				// head 업데이트
				teams[i].head = { nRow, nCol };
				for (int k = 0; k < 4; k++)
				{
					int eNRow = teams[i].tail.first + drow[k];
					int eNCol = teams[i].tail.second + dcol[k];
					if (eNRow > n || eNRow < 1 || eNCol > n || eNCol < 1) continue;
					// 내 2번인지 확인
					if (board[eNRow][eNCol] == 2)
					{
						//cout << eNRow << " " << eNCol << "이 2임\n";
						// 2로 이동후
						board[eNRow][eNCol] = 3;
						// 기존 3칸을 2로 채움
						//cout << teams[i].tail.first << " " << teams[i].tail.second << "를 4로 변경\n";
						board[teams[i].tail.first][teams[i].tail.second] = 4;
						// tail 업데이트
						teams[i].tail = { eNRow, eNCol };
						break;
					}
				}
				break;
			}
			else if (board[nRow][nCol] == 3)
			{
				//cout << "3\n";
				for (int k = 0; k < 4; k++)
				{
					int eNRow = teams[i].tail.first + drow[k];
					int eNCol = teams[i].tail.second + dcol[k];
					if (eNRow > n || eNRow < 1 || eNCol > n || eNCol < 1) continue;
					// 내 2번인지 확인
					if (board[eNRow][eNCol] == 2)
					{
						// 2로 이동후
						board[eNRow][eNCol] = 3;
						// 이전 위치에 1이 도착
						board[teams[i].tail.first][teams[i].tail.second] = 1;
						board[teams[i].head.first][teams[i].head.second] = 2;
						// tail 업데이트
						teams[i].head = { nRow, nCol };
						teams[i].tail = { eNRow, eNCol };
						break;
					}
				}
				break;
			}
		}
	}
}
void shootBall()
{
	int targetTeam;
	pair<int, int> pos;
	bool isFound = false;
	if (dir == 0)
	{
		for (int i = 1; i <= n; i++)
		{
			if (board[offset][i] > 0 && board[offset][i] != 4)
			{
				//cout << "found at (dir,offseT) " << dir << " " << offset << '\n';
				targetTeam = teamBoard[offset][i];
				pos = { offset ,i };
				isFound = true;
				break;
			}
		}
	}
	else if (dir == 1)
	{
		for (int i = n; i >= 1; i--)
		{
			if (board[i][offset] > 0 && board[i][offset] != 4)
			{
				targetTeam = teamBoard[i][offset];
				pos = { i, offset };
				isFound = true;
				break;
			}
		}
	}
	else if (dir == 2)
	{
		for (int i = n; i >= 1; i--)
		{
			if (board[offset][i] > 0 && board[offset][i] != 4)
			{
				targetTeam = teamBoard[n + 1 - offset][i];
				pos = { n + 1 - offset, i };
				isFound = true;
				break;
			}
		}
	}
	else if (dir == 3)
	{
		for (int i = 1; i <= 1; i++)
		{
			if (board[offset][i] > 0 && board[offset][i] != 4)
			{
				targetTeam = teamBoard[i][n + 1 - offset];
				pos = { i, n + 1 - offset };
				isFound = true;
				break;
			}
		}
	}
	if (isFound)
	{
		//cout << "attack!" << pos.first << " " << pos.second << "\n";
		//cout << "공격정보: row, col, dir" << pos.first << " " << pos.second << " " << dir << "\n";
		int ch[21][21] = { 0 };
		queue<pair<int, int> > q;
		q.push(teams[targetTeam].head);
		ch[teams[targetTeam].head.first][teams[targetTeam].head.second] = 1;
		int pathlen = 1;
		while (!q.empty())
		{
			pair<int, int> curpos = q.front();
			q.pop();
			if (pathlen < ch[curpos.first][curpos.second]) pathlen = ch[curpos.first][curpos.second];
			for (int i = 0; i < 4; i++)
			{
				int nRow = curpos.first + drow[i];
				int nCol = curpos.second + dcol[i];
				if (nRow > n || nRow < 1 || nCol > n || nCol < 1) continue;
				if (ch[nRow][nCol] == 0 && board[nRow][nCol] == 2)
				{
					ch[nRow][nCol] = ch[curpos.first][curpos.second] + 1;
					q.push({ nRow, nCol });
				}
			}
		}
		ch[teams[targetTeam].tail.first][teams[targetTeam].tail.second] = pathlen + 1;
		score += ch[pos.first][pos.second] * ch[pos.first][pos.second];
		// 공격당한 것 반전
		pair<int, int> headBack = teams[targetTeam].head;
		teams[targetTeam].head = teams[targetTeam].tail;
		teams[targetTeam].tail = headBack;
		board[teams[targetTeam].head.first][teams[targetTeam].head.second] = 1;
		board[teams[targetTeam].tail.first][teams[targetTeam].tail.second] = 3;
	}
	offset++;
	if (offset == n + 1)
	{
		offset = 1;
		dir++;
		if (dir == 4) dir = 0;
	}
}
int main()
{
	getInput();
	initTeam();
	for (turn = 1; turn <= k; turn++)
	{
		//cout << "턴 " << turn << "\n";
		//cout << "이전\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= n; j++)
		//	{
		//		cout << board[i][j] << "\t";
		//	}
		//	cout << "\n";
		//}
		//cout << score << "\n";
		moveTeam();
		//cout << "이동완료\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= n; j++)
		//	{
		//		cout << board[i][j] << "\t";
		//	}
		//	cout << "\n";
		//}
		//cout << score << "\n";
		shootBall();
		//cout << "턴종\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= n; j++)
		//	{
		//		cout << board[i][j] << "\t";
		//	}
		//	cout << "\n";
		//}
		//cout << score << "\n";
	}
	cout << score << "\n";
	//for (int i = 1; i <= m; i++)
	//{
	//	cout << "team " << i << " 시작: " << teams[i].head.first << " " << teams[i].head.second;
	//	cout << "끝: " << teams[i].tail.first << " " << teams[i].tail.second << "\n";
	//}
}