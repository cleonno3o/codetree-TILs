#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
int n, m, k;

int atk[11][11];
int lastAtk[11][11];
bool isPlayed[11][11];
//bool strongExist;
bool weakExist;
int turn;
int wr, wc, sr, sc;
int drow[] = { 0, 1, 0, -1 };
int dcol[] = { 1, 0, -1, 0 };
int drow8[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dcol8[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int backRow[] = { 1, 0, -1, 0 };
int backCol[] = { 0, 1, 0, -1 };
//struct CheckItem {
	//pair <int, int> pos = { 0,0 };
	//pair <int, int> prevPos = { 0, 0 };
	//CheckItem(pair<int, int> _pos, pair<int, int> _prevPos)
	//{
	//	pair <int, int> pos = _pos;
	//	pair <int, int> prevPos = _prevPos;
	//}
//};
//int ch[11][11];
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

	//cout << "weak 결과: " << wr << wc << '\n';

	return { wr, wc };
}
pair<int, int> selectStrong()
{
	int sAtk = -100;
	//strongExist = false;
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
					//strongExist = true;
				}
				else if (atk[i][j] == sAtk)
				{
					if (lastAtk[sr][sc] > lastAtk[i][j])
					{
						sr = i;
						sc = j;
						//strongExist = true;

					}
					else if (lastAtk[sr][sc] == lastAtk[i][j])
					{
						if (sr + sc > i + j)
						{
							sr = i;
							sc = j;
							//strongExist = true;

						}
						else if (sr + sc == i + j)
						{
							if (sc > j)
							{
								sr = i;
								sc = j;
								//strongExist = true;
							}
						}
					}
				}
			}
		}
	}
	//cout << "strong 결과: " << sr << sc << '\n';
	return { sr, sc };
}
void tryLaser()
{
	isAttacked = false;
	// 최단 거리 체크
	int chMin[11][11] = { 0 };
	queue<pair<int, int> > qMin;
	qMin.push({ wr, wc });
	chMin[wr][wc] = 1;
	while (!qMin.empty())
	{
		pair<int, int> pos = qMin.front();
		qMin.pop();
		if (pos.first == sr && pos.second == sc) break;
		for (int i = 0; i < 4; i++)
		{
			int nRow = pos.first + drow[i];
			int nCol = pos.second + dcol[i];
			if (nRow < 1) nRow = n;
			if (nRow > n) nRow = 1;
			if (nCol < 1) nCol = m;
			if (nCol > m) nCol = 1;
			if (chMin[nRow][nCol] == 0)
			{
				qMin.push({ nRow, nCol });
				chMin[nRow][nCol] = chMin[pos.first][pos.second];
			}
		}
	}
	int minDist = chMin[sr][sc];
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
	if (ch[sr][sc] == make_pair(-1, -1) || cdist > minDist) isAttacked = false;
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
				//cout << "현재 여기가 데미지 입습니다 " << prevPos.first << " " << prevPos.second << " ";
				//cout << atk[prevPos.first][prevPos.second] << "이 체력에서" << atk[wr][wc] / 2 << '\n';
				atk[prevPos.first][prevPos.second] -= (atk[wr][wc] / 2);
				isPlayed[prevPos.first][prevPos.second] = true;
				gor = prevPos.first;
				goc = prevPos.second;
			}
		}
		atk[sr][sc] -= atk[wr][wc];
		isPlayed[sr][sc] = true;
		//cout << "isPlayed\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= m; j++)
		//		cout << isPlayed[i][j] << '\t';
		//	cout << '\n';
		//}
		isAttacked = true;
		//int dist = ch[sr][sc];
		////cout << "거리: " << dist << '\n';
		//int r = sr;
		//int c = sc;
		//isAttacked = true;
		//// 역추적
		//queue<pair<int, int> > target;
		//for (int i = dist; i > 1; i--)
		//{
		//	for (int j = 0; j < 4; j++)
		//	{
		//		int nRow = r + backRow[j];
		//		int nCol = c + backCol[j];
		//		if (nRow < 1) nRow = n;
		//		if (nRow > n) nRow = 1;
		//		if (nCol < 1) nCol = m;
		//		if (nCol > m) nCol = 1;
		//		if (ch[nRow][nCol] == ch[r][c] - 1)
		//		{
		//			r = nRow;
		//			c = nCol;
		//			//cout << "여기로 가능중: " << r << " " << c << '\n';
		//			target.push({ r, c });
		//			break;
		//		}
		//	}
		//}
		//// 공격!
		//while (!target.empty())
		//{
		//	pair<int, int> pos = target.front();
		//	target.pop();			
		//	if (pos.first == wr && pos.second == wc)
		//	{
		//		continue;
		//	}
		//	atk[pos.first][pos.second] -= (atk[wr][wc] / 2);
		//	isPlayed[pos.first][pos.second] = true;
		//}
		//atk[sr][sc] -= atk[wr][wc];
		//isPlayed[sr][sc] = true;
		//cout << '\n';
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= m; j++)
		//		cout << atk[i][j] << '\t';
		//	cout << '\n';
		//}
	}
}
//void tryLaserDfs(int gor, int goc)
//{
//	if (gor == sr && goc == sc)
//	{
//		for (int i = 1; i <= n; i++)
//		{
//			for (int j = 1; j <= m; j++)
//			{
//				if (ch[i][j] == 1)
//				{
//					if (i == wr && j == wc) continue;
//					if (i == sr && j == sc) continue;
//					atk[i][j] -= (atk[wr][wc] / 2);
//				}
//			}
//		}
//		atk[sr][sc] -= atk[wr][wc];
//	}
//	else
//	{
//
//	}
//}
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
	//cout << "atk 보드 상황\n";
	//for (int i = 1; i <= n; i++)
	//{
	//	for (int j = 1; j <= m; j++)
	//		cout << atk[i][j] << "\t\t";
	//	cout << '\n';
	//}
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
		//cout << "결과\n";
		//for (int i = 1; i <= n; i++)
		//{
		//	for (int j = 1; j <= m; j++)
		//		cout << atk[i][j] << "\t\t";
		//	cout << '\n';
		//}
	}
	wr = wc = -1;
	pair<int, int> s = selectStrong();
	cout << atk[s.first][s.second];
}