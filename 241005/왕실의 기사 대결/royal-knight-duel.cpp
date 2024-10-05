#include <iostream>
#include <vector>
#define WALL 2
using namespace std;
struct Knight {
	int hp = 0;
	int damage = 0;
	pair<int, int> shield;
	int h = 0;
	int w = 0;
};
int l, n, q;
int r, c, h, w, k;
bool flag = true;
int drow[] = { -1, 0, 1, 0 };
int dcol[] = { 0, 1, 0, -1 };
vector<vector<int>> board, knightMap;
vector<Knight> knight;
void recur(int orderTarget, int kIdx, int dir);
int main()
{
	cin >> l >> n >> q;
	board = vector<vector<int>>(l + 1, vector<int>(l + 1));
	knightMap = vector<vector<int>>(l + 1, vector<int>(l + 1));
	knight = vector<Knight>(n + 1);
	for (int i = 1; i <= l; i++)
	{
		for (int j = 1; j <= l; j++)
		{
			cin >> board[i][j];
		}
	}
	for (int i = 1; i <= n; i++)
	{
		cin >> r >> c >> h >> w >> k;
		knight[i].hp = k;
		knight[i].shield = { r, c };
		knight[i].h = h;
		knight[i].w = w;
		knight[i].damage = 0;
		for (int j = r; j < r + h; j++)
		{
			for (int k = c; k < c + w; k++)
			{
				knightMap[j][k] = i;
			}
		}
	}
	for (int i = 1; i <= q; i++)
	{
		int target, dir;
		cin >> target >> dir;
		if (knight[target].hp > 0)
		{
			flag = true;
			recur(target, target, dir);
		}
	}
	int damageSum = 0;
	for (int i = 1; i <= n; i++)
	{
		if (knight[i].hp > 0) damageSum += knight[i].damage;
	}
	cout << damageSum;
}

void recur(int orderTarget, int kIdx, int dir)
{
	Knight& cur = knight[kIdx];
	vector<int> nextIdx;
	// 방향으로 현재 기사를 모두 한칸 이동
	for (int i = cur.shield.first; i < cur.shield.first + cur.h; i++)
	{
		for (int j = cur.shield.second; j < cur.shield.second + cur.w; j++)
		{
			int nRow = i + drow[dir];
			int nCol = j + dcol[dir];
			// 벽을 만나면 이번 명령은 무효
			if (nRow > l || nRow <= 0 ||
				nCol > l || nCol <= 0 ||
				board[nRow][nCol] == WALL)
			{
				flag = false;
				break;
			}
			// 기사를 만났으면 이동 대상에 추가
			else if (knightMap[nRow][nCol] != kIdx &&
				knightMap[nRow][nCol] >= 1 && knightMap[nRow][nCol] <= n)
			{
				nextIdx.push_back(knightMap[nRow][nCol]);
			}
		}
	}
	// 다음 기사가 있으면 재귀시작
	for (int i = 0; i < nextIdx.size(); i++)
		recur(orderTarget, nextIdx[i], dir);
	// 확인이 끝남
	// 그동안 벽을 안만났으면 내가 이동
	if (flag)
	{
		int damageCnt = 0;
		// 이전자리 비우기
		for (int i = cur.shield.first; i < cur.shield.first + cur.h; i++)
		{
			for (int j = cur.shield.second; j < cur.shield.second + cur.w; j++)
				knightMap[i][j] = 0;
		}
		// 새자리로 이동
		for (int i = cur.shield.first; i < cur.shield.first + cur.h; i++)
		{
			for (int j = cur.shield.second; j < cur.shield.second + cur.w; j++)
			{
				knightMap[i + drow[dir]][j + dcol[dir]] = kIdx;
				if (board[i + drow[dir]][j + dcol[dir]] == 1)
					damageCnt++;
			}
		}
		cur.shield = { cur.shield.first + drow[dir], cur.shield.second + dcol[dir] };
		if (kIdx != orderTarget)
		{
			cur.hp -= damageCnt;
			cur.damage += damageCnt;
		}
		if (cur.hp <= 0)
		{
			for (int i = cur.shield.first; i < cur.shield.first + cur.h; i++)
			{
				for (int j = cur.shield.second; j < cur.shield.second + cur.w; j++)
				{
					knightMap[i][j] = 0;
				}
			}
		}
	}
}