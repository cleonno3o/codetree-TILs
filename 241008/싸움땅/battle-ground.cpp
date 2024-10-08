#include <iostream>
#include <queue>
using namespace std;
int drow[] = { -1,0,1,0 };
int dcol[] = { 0,1,0,-1 };
int n, m, k;
priority_queue<int> board[21][21];
int playerBoard[21][21];
pair<int, int> player[31];
int ability[31];
int dir[31];
int score[31];
int gun[31];
void UpdatePlayerInfo(int pIdx, int nRow, int nCol)
{
	playerBoard[player[pIdx].first][player[pIdx].second] = 0;
	player[pIdx].first = nRow;
	player[pIdx].second = nCol;
}
void updatePlyerBoard(int pIdx, int nRow, int nCol)
{
	playerBoard[nRow][nCol] = pIdx;
}
void initValue()
{
	cin >> n >> m >> k;
	// 보드 입력
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			int temp;
			cin >> temp;
			board[i][j].push(temp);
		}
	}
	 //플레이어 정보 입력
	for (int i = 1; i <= m; i++)
	{
		int x, y, d, s;
		cin >> x >> y >> d >> s;
		player[i] = make_pair(x, y);
		playerBoard[x][y] = i;
		dir[i] = d;
		ability[i] = s;
	}
}
void getGun(int playerIdx, int row, int col)
{
	UpdatePlayerInfo(playerIdx, row, col);
	// 보드의 총이 더 세면
	if (!board[row][col].empty() && gun[playerIdx] < board[row][col].top())
	{
		// 내 총을 내려놓고 주움
		int prevGun = gun[playerIdx];
		gun[playerIdx] = board[row][col].top();
		board[row][col].pop();
		board[row][col].push(prevGun);
		// 이동 종료 내위치 업데이트
	}
	updatePlyerBoard(playerIdx, row, col);
}
int rotateClock(int dir)
{
	switch (dir)
	{
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 3;
	case 3:
		return 0;
	}
}
void fight(int p1, int p2, int row, int col)
{
	int winner, loser;
	UpdatePlayerInfo(p1, row, col);
	UpdatePlayerInfo(p2, row, col);
	if (ability[p1] + gun[p1] > ability[p2] + gun[p2])
	{
		winner = p1;
		loser = p2;
	}
	else if (ability[p1] + gun[p1] < ability[p2] + gun[p2])
	{
		winner = p2;
		loser = p1;
	}
	else
	{
		if (ability[p1] > ability[p2])
		{
			winner = p1;
			loser = p2;
		}
		else
		{
			winner = p2;
			loser = p1;
		}
	}
	// 승자는 점수를 얻음
	int point = (ability[winner] + gun[winner]) - (ability[loser] + gun[loser]);
	score[winner] += point;
	// 패자는 플레이어가 없는곳으로 이동하며 총을 주움
	// 유효하지 않은 곳(범위밖, 플레이어가있음)이면 90도 회전하며 빈칸을 찾음
	board[row][col].push(gun[loser]);
	gun[loser] = 0;
	int nRow, nCol;
	while (1)
	{
		nRow = row + drow[dir[loser]];
		nCol = col + dcol[dir[loser]];
		if (nRow > n || nRow < 1 || nCol > n || nCol < 1 || playerBoard[nRow][nCol] > 0)
		{
			dir[loser] = rotateClock(dir[loser]);
		}
		else break;
	}
	getGun(loser, nRow, nCol);
	// 승자는 버린총을 비교
	getGun(winner, row, col);
	// 승자 패자 위치 업데이트
	updatePlyerBoard(winner, row, col);
	updatePlyerBoard(loser, nRow, nCol);
}
int toOpposite(int dir)
{
	switch (dir)
	{
	case 1:
		return 3;
	case 2:
		return 0;
	case 3:
		return 1;
	case 0:
		return 2;
	}
}
void movePlayer()
{
	for (int i = 1; i <= m; i++)
	{
		// 자기 방향으로 한칸 이동
		int nRow = player[i].first + drow[dir[i]];
		int nCol = player[i].second + dcol[dir[i]];
		// 벗어나는 범위면 반대방향으로
		if (nRow > n || nRow < 1 || nCol > n || nCol < 1)
		{
			dir[i] = toOpposite(dir[i]);
			nRow = player[i].first + drow[dir[i]];
			nCol = player[i].second + dcol[dir[i]];
		}
		// 다음칸이 플레이어면
		if (playerBoard[nRow][nCol] > 0)
		{
			fight(i, playerBoard[nRow][nCol], nRow, nCol);
		}
		else
		{
			getGun(i, nRow, nCol);
		}
	}
}
int main()
{
	initValue();
	for (int i = 1; i <= k; i++)
	{
		movePlayer();
    }
	// 최종 점수 출력
	for (int i = 1; i <= m; i++)
	{
		cout << score[i] << " ";
	}
}