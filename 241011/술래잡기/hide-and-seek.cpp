// 술래잡기
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Runner {
	int row = 0;
	int col = 0;
	int type = 0;
	int dir = 0;
	Runner(int _row, int _col, int _type)
		: row(_row), col(_col), type(_type) {}
};
int N, M, H, K;
bool isTree[100][100];
int isVisit[100][100];
int drow[3][3] = { {-1000 ,-1000 }, { 0, 0 }, { 1, -1 } };
int dcol[3][3] = { {-1000, -1000 }, { 1, -1 }, { 0, 0 } };
int dkrow[] = { -1, 0, 1, 0 };
int dkcol[] = { 0, 1, 0, -1 };
int dkrowBack[] = { 1, 0, -1, 0 };
int dkcolBack[] = { 0, 1, 0, -1 };
int killerDir = 0;
int turn = 1;
queue<Runner> runner;
pair<int, int> killer;
pair<int, int> targets[3];
bool isBack = false;
int walked = 0;
int score = 0;
void getInput()
{
	cin >> N >> M >> H >> K;
	for (int i = 0; i < M; i++)
	{
		int row, col, type;
		cin >> row >> col >> type;
		runner.push(Runner(row, col, type));
	}
	for (int i = 0; i < H; i++)
	{
		int row, col;
		cin >> row >> col;
		isTree[row][col] = true;
	}
	killer = make_pair(N / 2 + 1, N / 2 + 1);
}
int rotateKiller()
{
	int res = killerDir + 1;
	if (res == 4) res = 0;
	return res;
}
void moveRunner()
{
	int runCnt = runner.size();
	for (int i = 0; i < runCnt; i++)
	{
		Runner player = runner.front();
		runner.pop();
		// 거리계산
		int dist = abs(player.row - killer.first) + abs(player.col - killer.second);
		if (dist > 3)
		{
			runner.push(player);
			continue;
		}
		// 3 이하면 이동
		else
		{
			int nRow = player.row + drow[player.type][player.dir];
			int nCol = player.col + dcol[player.type][player.dir];
			// 다음 칸이 보드 밖이면
			if (nRow > N || nRow < 1 || nCol > N || nCol < 1)
			{
				// 방향을 바꾸고 이동 시도
				player.dir = (player.dir == 0 ? 1 : 0);
				nRow = player.row + drow[player.type][player.dir];
				nCol = player.col + dcol[player.type][player.dir];
			}
			// 킬러 위치가 아니면 이동
			if (nRow != killer.first || nCol != killer.second)
			{
				player.row = nRow;
				player.col = nCol;
			}
			// 킬러 위치였으면 업데이트 하지않고 push 하게 됨
			runner.push(player);
		}
	}
}
void moveKiller()
{
	if (!isBack)
	{
		if (walked == 0)
		{
			isVisit[killer.first][killer.second] = 1;
			killer.first += dkrow[killerDir];
			killer.second += dkcol[killerDir];
			isVisit[killer.first][killer.second] = 1;
			killerDir = rotateKiller();
			walked++;
		}
		else if (walked == N * N - 2)
		{
			int nRow = killer.first + dkrow[killerDir];
			int nCol = killer.second + dkcol[killerDir];
			killer = make_pair(nRow, nCol);
			isVisit[nRow][nCol] = 1;

			isBack = true;
			killerDir = 0;
			walked++;
		}

		else
		{
			int nRow = killer.first + dkrow[killerDir];
			int nCol = killer.second + dkcol[killerDir];
			killer = make_pair(nRow, nCol);
			isVisit[nRow][nCol] = 1;

			int nextDir = rotateKiller();
			int nextDirRow = killer.first + dkrow[nextDir];
			int nextDirCol = killer.second + dkcol[nextDir];
			if (isVisit[nextDirRow][nextDirCol] == 0) killerDir = nextDir;
			walked++;
		}
	}
	else
	{
		if (walked != 1)
		{
			isVisit[killer.first][killer.second] = 0;
			int nRow = killer.first + dkrowBack[killerDir];
			int nCol = killer.second + dkcolBack[killerDir];
			killer = make_pair(nRow, nCol);

			int nextRow = killer.first + dkrowBack[killerDir];
			int nextCol = killer.second + dkcolBack[killerDir];
			if (nextRow > N || nextRow < 1 || nextCol > N || nextCol < 1)
				killerDir = rotateKiller();
			else if (isVisit[nextRow][nextCol] == 0)
				killerDir = rotateKiller();
			walked--;
		}
		else if (walked == 1)
		{
			isVisit[killer.first][killer.second] = 0;
			int nRow = killer.first + dkrowBack[killerDir];
			int nCol = killer.second + dkcolBack[killerDir];
			killer = make_pair(nRow, nCol);
			isBack = false;
			killerDir = 0;
			walked--;
		}
	}

}
void searchRunner()
{
	if (!isBack)
	{
		targets[0] = killer;
		targets[1] = make_pair(targets[0].first + dkrow[killerDir], targets[0].second + dkcol[killerDir]);
		targets[2] = make_pair(targets[1].first + dkrow[killerDir], targets[1].second + dkcol[killerDir]);
	}
	if (isBack)
	{
		targets[0] = killer;
		targets[1] = make_pair(targets[0].first + dkrowBack[killerDir], targets[0].second + dkcolBack[killerDir]);
		targets[2] = make_pair(targets[1].first + dkrowBack[killerDir], targets[1].second + dkcolBack[killerDir]);
	}
}
void killRunner()
{
	int kill = 0;
	for (int i = 0; i < 3; i++)
	{
		pair<int, int> pos = targets[i];
		if (pos.first > N || pos.first < 1 || pos.second > N || pos.second < 1 || isTree[pos.first][pos.second]) continue;
		int runnerCnt = runner.size();
		for (int j = 0; j < runnerCnt; j++)
		{
			Runner mayDie = runner.front();
			runner.pop();
			if (make_pair(mayDie.row, mayDie.col) == pos)
			{
				kill++;
			}
			else
				runner.push(mayDie);
		}
	}
	score += turn * kill;
	//cout << score << "\n";
}
int main()
{
	getInput();
	for (turn = 1; turn <= K; turn++)
	{
		moveRunner();
		moveKiller();
		searchRunner();
		//cout << "[[[[" << walked << "]]]]\n";
		//for (int j = 1; j <= N; j++)
		//{
		//	for (int k = 1; k <= N; k++)
		//	{
		//		if (killer == make_pair(j, k))
		//			cout << "K" << "\t";
		//		else if (isVisit[j][k] == 1) cout << "1" << "\t";
		//		else cout << "0" << "\t";
		//	}
		//	cout << '\n';
		//}
		//cout << "검사 대상\n";
		//for (int i = 0; i < 3; i++)
		//	cout << targets[i].first << ' ' << targets[i].second << ' ';
		//cout << "\n";
		killRunner();
	}
	cout << score;
}