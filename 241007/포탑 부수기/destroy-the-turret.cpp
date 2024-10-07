#include <iostream>
#include <queue>
using namespace std;
int n, m, k;

int atk[11][11];
int lastAtk[11][11];
bool isPlayed[11][11];
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
	//cout << "지난공격\n";
	//for (int i = 1; i <= n; i++)
	//{
	//	for (int j = 1; j <= m; j++)
	//		cout << lastAtk[i][j] << '\t';
	//	cout << '\n';
	//}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (atk[i][j] > 0)
			{
				if (atk[i][j] < weakAtk)
				{
					weakAtk = atk[i][j];
					//cout << "weak보다 작은 크기" << i << j << '\n';
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
						else if (wr + wc < i + j)
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
	atk[wr][wc] += n + m;
	lastAtk[wr][wc] = turn;
	isPlayed[wr][wc] = true;
	//cout << "weak 결과: " << wr << wc << '\n';
	//cout << '\n';
	//for (int i = 1; i <= n; i++)
	//{
	//	for (int j = 1; j <= m; j++)
	//		cout << atk[i][j] << '\t';
	//	cout << '\n';
	//}
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
	isAttacked = true;
	// 도착지점까지 경로 구하기
	int ch[11][11] = { 0 };
	queue<pair<int, int> > q;
	q.push({ wr, wc });
	ch[wr][wc] = 1;
	while (!q.empty())
	{
		pair<int, int> pos = q.front();
		q.pop();
		for (int i = 0; i < 4; i++)
		{
			int nRow = pos.first + drow[i];
			int nCol = pos.second + dcol[i];
			if (nRow < 1) nRow = n;
			if (nRow > n) nRow = 1;
			if (nCol < 1) nCol = m;
			if (nCol > m) nCol = 1;
			if (ch[nRow][nCol] == 0 && atk[nRow][nCol] > 0)
			{
				q.push({ nRow, nCol });
				ch[nRow][nCol] = ch[pos.first][pos.second] + 1;
			}
		}
	}
	//cout << "ch배열\n";
	//for (int i = 1; i <= n; i++)
	//{
	//	for (int j = 1; j <= m; j++)
	//		cout << ch[i][j] << '\t';
	//	cout << '\n';
	//}
	// 경로가 없으면 공격하지 않음
	if (ch[sr][sc] == 0) isAttacked = false;
	// 경로가 존재하면
	else
	{
		int dist = ch[sr][sc];
		//cout << "거리: " << dist << '\n';
		int r = sr;
		int c = sc;
		isAttacked = true;
		// 역추적
		queue<pair<int, int> > target;
		for (int i = dist; i > 2; i--)
		{
			for (int j = 0; j < 4; j++)
			{
				int nRow = r + backRow[j];
				int nCol = c + backCol[j];
				if (nRow < 1) nRow = n;
				if (nRow > n) nRow = 1;
				if (nCol < 1) nCol = m;
				if (nCol > m) nCol = 1;
				if (ch[nRow][nCol] == ch[r][c] - 1)
				{
					r = nRow;
					c = nCol;
					//cout << "여기로 가능중: " << r << " " << c << '\n';
					target.push({ r, c });
					break;
				}
			}
		}
		// 공격!
		while (!target.empty())
		{
			pair<int, int> pos = target.front();
			target.pop();			
			atk[pos.first][pos.second] -= (atk[wr][wc] / 2);
			isPlayed[pos.first][pos.second] = true;
		}
		atk[sr][sc] -= atk[wr][wc];
		isPlayed[sr][sc] = true;
		//cout << '\n';
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= m; j++)
		//		cout << atk[i][j] << '\t';
		//	cout << '\n';
		//}
	}
}
void tryBomb()
{
	if (!isAttacked)
	{
		//cout << "레이저 실패" << '\n';
		// 8방위 공격!
		for (int i = 0; i < 8; i++)
		{
			int nRow = sr + drow8[i];
			int nCol = sc + dcol8[i];
			if (nRow < 1) nRow = n;
			if (nRow > n) nRow = 1;
			if (nCol < 1) nCol = m;
			if (nCol > m) nCol = 1;
			if (atk[nRow][nCol] > 0)
			{
				atk[nRow][nCol] -= atk[wr][wc] / 2;
				isPlayed[nRow][nCol] = true;
			}
		}
		atk[sr][sc] -= atk[wr][wc];
		isPlayed[sr][sc] = true;
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
				//cout <<"똑같노 " << i << j << '\n';
				continue;
			}

			if (!isPlayed[i][j] && atk[i][j] > 0)
			{
				//cout << "더하기" << i << j<< '\n';
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
		playTurn();
		//cout << "결과\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= m; j++)
		//		cout << atk[i][j] << '\t';
		//	cout << '\n';
		//}
	}
	wr = wc = -1;
	pair<int, int> s = selectStrong();
	cout << atk[s.first][s.second];
}