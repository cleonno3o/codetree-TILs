#include <iostream>
#include <queue>
using namespace std;

int drow[] = { -1000, 0, -1, -1, -1,  0,  1, 1, 1 };
int dcol[] = { -1000, 1,  1,  0, -1, -1, -1, 0, 1 };
int drugrow[] = { -1, -1, 1, 1 };
int drugcol[] = { -1, 1, -1, 1 };
int N, M;
int board[16][16];
bool isTarget[16][16];
int dir, dist;
queue<pair<int, int> > drug;
void getInput()
{
	cin >> N >> M;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cin >> board[i][j];
	}
	drug.push(make_pair(N - 1, 0));
	drug.push(make_pair(N - 1, 1));
	drug.push(make_pair(N - 2, 0));
	drug.push(make_pair(N - 2, 1));
}
void moveDrug()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			isTarget[i][j] = false;
	}
	int rowOffset = drow[dir] * (dist % N);
	int colOffset = dcol[dir] * (dist % N);
	int drugCnt = drug.size();
	for (int i = 0; i < drugCnt; i++)
	{
		pair<int, int> pos = drug.front();
		drug.pop();
		int nRow = pos.first + rowOffset;
		int nCol = pos.second + colOffset;
		if (nRow >= N) nRow -= N;
		else if (nRow < 0) nRow += N;
		if (nCol >= N) nCol -= N;
		else if (nCol < 0) nCol += N;
		isTarget[nRow][nCol] = true;
		board[nRow][nCol]++;
		drug.push({nRow, nCol});
	}
	for (int i = 0; i < drugCnt; i++)
	{
		pair<int, int> pos = drug.front();
		//cout << pos.first << " " << pos.second <<"\n";
		drug.pop();
		int grow = 0;
		for (int j = 0; j < 4; j++)
		{
			int nearRow = pos.first + drugrow[j];
			int nearCol = pos.second + drugcol[j];
			if (nearRow >= N || nearRow < 0 || 
				nearCol >= N || nearCol < 0) continue;
			if (board[nearRow][nearCol] != 0) grow++;
		}
		board[pos.first][pos.second] += grow;
	}
}
void putNewDrug()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (board[i][j] >= 2 && isTarget[i][j] == false)
			{
				board[i][j] -= 2;
				drug.push({ i, j });
			}
		}
	}
}
void printHeight()
{
	int res = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			res += board[i][j];
			//cout << board[i][j] << " ";
		}
		//cout << "\n";
	}
	cout << res;
}
int main()
{
	getInput();
	for (int i = 0; i < M; i++)
	{
		cin >> dir >> dist;
		moveDrug();
		putNewDrug();
	}
	printHeight();
}