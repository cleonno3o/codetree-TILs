#include <iostream>
#include <queue>
using namespace std;

struct Egg {
	int r;
	int c;
	int dir;
	Egg() = default;
	Egg(int _r, int _c, int _dir)
		: r(_r), c(_c), dir(_dir) {}
};
int m, t;
pair<int, int> pac;
queue<int> mon[5][5];
queue<int> dead[5][5];
pair<int, int> path[4];
pair<int, int> maxPath[4];
int maxEat = -1;
int drow[] = { -100, -1, -1, 0, 1, 1, 1, 0, -1 };
int dcol[] = { -100, 0, -1, -1, -1, 0, 1, 1, 1 };
int pacrow[] = { -1, 0, 1, 0 };
int paccol[] = { 0, -1, 0, 1 };
int chPac[5][5];
queue<Egg> eggs;
void putEgg()
{
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			int monCnt = (int) mon[i][j].size();
			for (int k = 1; k <= monCnt; k++)
			{
				int dir = mon[i][j].front();
				mon[i][j].pop();
				mon[i][j].push(dir);
				eggs.push(Egg(i, j, dir));
			}
		}
	}
}
int rotate45(int dir)
{
	dir++;
	if (dir == 9) dir = 1;
	return dir;
}
void tryMove()
{
	queue<int> newMon[5][5];
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			int monCnt = (int) mon[i][j].size();
			for (int k = 1; k <= monCnt; k++)
			{
				//cout << i << j << " 수: " << monCnt << "\n";
				int dir = mon[i][j].front();
				mon[i][j].pop();
				int nRow, nCol;
				bool isMove = false;
				//cout << "원래 방향 " << dir << " ";
				for (int l = 1; l <= 8; l++)
				{
					nRow = i + drow[dir];
					nCol = j + dcol[dir];
					// 초과 / 시체 / 팩맨인 경우 회전
					if (nRow > 4 || nRow < 1 || nCol > 4 || nCol < 1 ||
						!dead[nRow][nCol].empty() ||
						(nRow == pac.first && nCol == pac.second))
					{
						dir = rotate45(dir);
					}
					else
					{
						isMove = true;
						break;
					}
				}
				//cout << i << " " << j << "가 " << dir << "로\n";
				if (isMove)
				{
					newMon[nRow][nCol].push(dir);
				}
				else
				{
					newMon[i][j].push(dir);
				}
			}
		}
	}
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
			mon[i][j] = newMon[i][j];
	}
}
void movePac(int row, int col, int pathlen, int eat)
{
	if (pathlen == 0)
	{
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
				chPac[i][j] = mon[i]	[j].size();
		}
		maxEat = -1;
	}
	if (pathlen == 3)
	{
		if (eat > maxEat)
		{
			maxEat = eat;
			for (int i = 1; i <= 3; i++)
				maxPath[i] = path[i];
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int nRow = row + pacrow[i];
			int nCol = col + paccol[i];
			if (nRow > 4 || nRow < 1 || nCol > 4 || nCol < 1) continue;
			//if (chPac[nRow][nCol]) continue;
			int back = chPac[nRow][nCol];
			eat += (int) back;
			chPac[nRow][nCol] = 0;
			path[pathlen + 1] = { nRow, nCol };
			movePac(nRow, nCol, pathlen + 1, eat);
			eat -= (int) back;
			path[pathlen + 1] = { -1, -1 };
			chPac[nRow][nCol] = back;
		}
	}
}
void eatMonAndMove()
{
	//cout << "경로: \n";
	//cout << maxEat << "\n";
	for (int i = 1; i <= 3; i++)
	{
		pair<int, int> pos = maxPath[i];
		//cout << pos.first << " " << pos.second << "\n";
		queue<int>& mq = mon[pos.first][pos.second];
		while (!mq.empty())
		{
			mq.pop();
			dead[pos.first][pos.second].push(3);
		}
	}
	pac = maxPath[3];
	//cout << "팩맨\n";
	//cout << pac.first << " " << pac.second << "\n";
}
void cleanDead()
{
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			int deadCnt = (int) dead[i][j].size();
			for (int k = 1; k <= deadCnt; k++)
			{
				int due = dead[i][j].front();
				dead[i][j].pop();
				due--;
				if (due > 0) dead[i][j].push(due);
			}
		}
	}
}
void spawnMon()
{
	while (!eggs.empty())
	{
		Egg egg = eggs.front();
		//cout << egg.r << " " << egg.c << "가 부화 방향 " << egg.dir << "\n";
		mon[egg.r][egg.c].push(egg.dir);
		eggs.pop();
	}
}
int main()
{
	cin >> m >> t;
	int row, col;
	cin >> row >> col;
	pac = { row, col };
	for (int i = 1; i <= m; i++)
	{
		int r, c, d;
		cin >> r >> c >> d;
		mon[r][c].push(d);
	}
	for (int i = 1; i <= t; i++)
	{
		//cout << i << "===========\n";
		//for (int k = 1; k <= 4; k++)
		//{
		//	for (int j = 1; j <= 4; j++)
		//		cout << mon[k][j].size() << " ";
		//	cout << "\n";
		//};
		//cout << "===\n";

		putEgg();
		tryMove();
		//for (int k = 1; k <= 4; k++)
		//{
		//	for (int j = 1; j <= 4; j++)
		//		cout << mon[k][j].size() << " ";
		//	cout << "\n";
		//}
		//cout << "===\n";
		movePac(pac.first, pac.second, 0, 0);
		eatMonAndMove();
		cleanDead();
		spawnMon();
		//for (int k = 1; k <= 4; k++)
		//{
		//	for (int j = 1; j <= 4; j++)
		//		cout << dead[k][j].size() << " ";
		//	cout << "\n";
		//}
		//cout << "최대경로\n";
		//for (int k = 1; k <= 3; k++)
		//	cout << maxPath[i].first << " " << maxPath[i].second << "\n";
	}
	int res = 0;
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
			res += (int) mon[i][j].size();
	}
	cout << res;
}