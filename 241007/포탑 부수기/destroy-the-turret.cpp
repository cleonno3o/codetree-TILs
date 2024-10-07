#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
int n, m, k;

int atk[11][11];
int lastAtk[11][11];
bool isPlayed[11][11];
bool weakExist;
int turn;
int wr, wc, sr, sc;
int drow[] = { 0, 1, 0, -1 };
int dcol[] = { 1, 0, -1, 0 };
int drow8[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dcol8[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int backRow[] = { 1, 0, -1, 0 };
int backCol[] = { 0, 1, 0, -1 };
bool isAttacked = false;
pair<int, int> selectWeak()
{
	int weakAtk = 6000;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (atk[i][j] > 0)
			{
				if (atk[i][j] < weakAtk)
				{
					weakAtk = atk[i][j];
					wr = i;
					wc = j;
				}
				else if (atk[i][j] == weakAtk)
				{
					if (lastAtk[wr][wc] < lastAtk[i][j])
					{
						wr = i;
						wc = j;
					}
					else if (lastAtk[i][j] == lastAtk[wr][wc])
					{
						if (wr + wc < i + j)
						{
							wr = i;
							wc = j;
						}
						else if (wr + wc == i + j)
						{
							if (wc < j)
							{
								wr = i;
								wc = j;
							}
						}
					}
				}
			}
		}
	}
	atk[wr][wc] += (n + m);
	lastAtk[wr][wc] = turn;
	isPlayed[wr][wc] = true;
	return { wr, wc };
}
pair<int, int> selectStrong()
{
	int sAtk = -100;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (i == wr && j == wc)
			{
				continue;
			}
			if (atk[i][j] > 0)
			{
				if (atk[i][j] > sAtk)
				{
					sAtk = atk[i][j];
					sr = i;
					sc = j;
				}
				else if (atk[i][j] == sAtk)
				{
					if (lastAtk[sr][sc] > lastAtk[i][j])
					{
						sr = i;
						sc = j;
					}
					else if (lastAtk[sr][sc] == lastAtk[i][j])
					{
						if (sr + sc > i + j)
						{
							sr = i;
							sc = j;
						}
						else if (sr + sc == i + j)
						{
							if (sc > j)
							{
								sr = i;
								sc = j;
							}
						}
					}
				}
			}
		}
	}
	return { sr, sc };
}
void tryLaser()
{
	isAttacked = false;
	// 도착지점까지 경로 구하기
	pair<int, int> ch[11][11];
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			ch[i][j] = { -1, -1 };
		}
	}
	queue<pair<pair<int, int>, int> > q;
	q.push({ { wr, wc }, 1 });
	ch[wr][wc] = { wr, wc };
	int cdist = -1;
	while (!q.empty())
	{
		pair<pair<int, int>, int> pos = q.front();
		q.pop();
		if (pos.first.first == sr && pos.first.second == sc)
		{
			cdist = pos.second;
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			int nRow = pos.first.first + drow[i];
			int nCol = pos.first.second + dcol[i];
			if (nRow < 1) nRow = n;
			if (nRow > n) nRow = 1;
			if (nCol < 1) nCol = m;
			if (nCol > m) nCol = 1;
			if (ch[nRow][nCol] == make_pair(-1,-1) && atk[nRow][nCol] > 0)
			{
				q.push({ { nRow, nCol }, pos.second });
				ch[nRow][nCol] = { pos.first.first, pos.first.second };
			}
		}
	}
	// 경로가 없으면 공격하지 않음
	if (ch[sr][sc] == make_pair(-1, -1)) isAttacked = false;
	// 경로가 존재하면
	else
	{
		int gor = sr;
		int goc = sc;
		while (1)
		{
			pair<int, int> prevPos = ch[gor][goc];
			if (prevPos == make_pair(wr, wc))
				break;
			else
			{
				atk[prevPos.first][prevPos.second] -= (atk[wr][wc] / 2);
				isPlayed[prevPos.first][prevPos.second] = true;
				gor = prevPos.first;
				goc = prevPos.second;
			}
		}
		atk[sr][sc] -= atk[wr][wc];
		isPlayed[sr][sc] = true;
		isAttacked = true;
	}
}
void tryBomb()
{
	if (!isAttacked)
	{
		// 8방위 공격!
		for (int i = 0; i < 8; i++)
		{
			int nRow = sr + drow8[i];
			int nCol = sc + dcol8[i];
			if (nRow < 1) nRow = n;
			if (nRow > n) nRow = 1;
			if (nCol < 1) nCol = m;
			if (nCol > m) nCol = 1;
			if (nRow == wr && nCol == wc) continue;
			if (atk[nRow][nCol] > 0)
			{
				atk[nRow][nCol] -= atk[wr][wc] / 2;
				isPlayed[nRow][nCol] = true;
			}
		}
		atk[sr][sc] -= atk[wr][wc];
		isPlayed[sr][sc] = true;
		isAttacked = true;
	}
}
void repair()
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (i == wr && j == wc)
			{
				continue;
			}
			if (atk[i][j] < 0)
			{
				atk[i][j] = 0;
				continue;
			}
			if (!isPlayed[i][j] && atk[i][j] > 0)
			{
				atk[i][j]++;
			}
		}
	}
}
void playTurn()
{
	// 참여했는지 확인하는 배열 초기화
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
			isPlayed[i][j] = false;
	}
	selectWeak();
	selectStrong();
	tryLaser();
	tryBomb();
	repair();
}
int main()
{
	cin >> n >> m >> k;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cin >> atk[i][j];
		}
	}
	for (turn = 1; turn <= k; turn++)
	{
		//cout << "turn: " << turn << "\n";
		int cnt = 0;
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= m;j++)
			{
				if (atk[i][j] > 0) cnt++;
			}
		}
		if (cnt == 1) break;
		playTurn();
	}
	wr = wc = -1;
	pair<int, int> s = selectStrong();
	cout << atk[s.first][s.second];
}