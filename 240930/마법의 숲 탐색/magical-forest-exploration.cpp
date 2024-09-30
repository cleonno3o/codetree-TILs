#include <iostream>
#include <vector>
using namespace std;
#define OFFSET 3
int drow[] = { -1, 0, 1, 0 };
int dcol[] = { 0, 1, 0, -1 };
int res;
int r, c, k;
int tempRes;
void dfs(int row, int col, int golemIdx, 
	vector<vector<int>>& forest, vector<vector<int>>& ch)
{
	ch[row][col] = 1;
	//cout << row << ' ' << col << '\n';
	//cout << forest[row][col] << '\n';
	if (tempRes < row) tempRes = row;
	for (int i = 0; i < 4; i++)
	{
		int nRow = row + drow[i];
		int nCol = col + dcol[i];
		if (
			nRow < forest.size() && nRow >= 1 &&
			nCol < forest[row].size() && nCol >= 1 &&
			ch[nRow][nCol] == 0
			)
		{
			if (forest[row][col] > 0)
			{
				if (forest[nRow][nCol] == golemIdx || forest[nRow][nCol] == -golemIdx)
					dfs(nRow, nCol, golemIdx, forest, ch);
			}
			else if (forest[row][col] < 0)
				dfs(nRow, nCol, forest[nRow][nCol], forest, ch);
		}
	}
}
bool checkDown(int centerRow, int centerCol, vector<vector<int>>& forest)
{
	if (centerRow < forest.size() - 2 &&
		forest[centerRow + 1][centerCol - 1] == 0 &&
		forest[centerRow + 2][centerCol] == 0 &&
		forest[centerRow + 1][centerCol + 1] == 0
		)
		return true;
	else
		return false;
}
bool checkLeft(int centerRow, int centerCol, vector<vector<int>>& forest)
{
	if (centerRow < forest.size() - 2 &&
		centerCol >= 3 &&
		forest[centerRow - 1][centerCol - 1] == 0 &&
		forest[centerRow][centerCol - 2] == 0 &&
		forest[centerRow + 1][centerCol - 2] == 0 &&
		forest[centerRow + 1][centerCol - 1] == 0 &&
		forest[centerRow + 2][centerCol - 1] == 0
		)
		return true;
	else
		return false;
}
bool checkRight(int centerRow, int centerCol, vector<vector<int>>& forest)
{
	if (centerRow < forest.size() - 2 &&
		centerCol <= c - 2 &&
		forest[centerRow - 1][centerCol + 1] == 0 &&
		forest[centerRow][centerCol + 2] == 0 &&
		forest[centerRow + 1][centerCol + 2] == 0 &&
		forest[centerRow + 1][centerCol + 1] == 0 &&
		forest[centerRow + 2][centerCol + 1] == 0
		)
		return true;
	else
		return false;
}
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> r >> c >> k;
	vector<vector<int>> forest(r + 1 + OFFSET, vector<int>(c + 1));
	for (int i = 1; i <= k; i++)
	{
		int col, out;
		cin >> col >> out;
		int golemIdx = i;
		int centerRow = 2;
		int centerCol = col;
		tempRes = 0;
		while (1)
		{
			if (checkDown(centerRow, centerCol, forest))
				centerRow++;
			else if (checkLeft(centerRow, centerCol, forest))
			{
				centerRow++;
				centerCol--;
				out--;
				if (out < 0)
					out = 3;
			}
			else if (checkRight(centerRow, centerCol, forest))
			{
				centerRow++;
				centerCol++;
				out++;
				if (out > 3)
					out = 0;
			}
			else
				break;
			//cout << centerRow << ' ' << centerCol << '\n';
		}
		if (centerRow < 5)
		{
			for (int i = 1; i < forest.size(); i++)
				for (int j = 1; j < forest[i].size(); j++)
					forest[i][j] = 0;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				forest[centerRow + drow[i]][centerCol + dcol[i]] = golemIdx;
			forest[centerRow][centerCol] = golemIdx;
			forest[centerRow + drow[out]][centerCol + dcol[out]] = -golemIdx;
			vector<vector<int>> ch(r + 1 + OFFSET, vector<int>(c + 1));
			dfs(centerRow, centerCol, golemIdx, forest, ch);
			res += tempRes - OFFSET;
			//cout << "res: " << res << '\n';
		}
	}
    cout << res;
}