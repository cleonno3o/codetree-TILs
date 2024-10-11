#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N, K;
int turn = 0;
int drow[] = { 0, 0, 1, -1 };
int dcol[] = { 1, -1, 0, 0 };
vector<vector<int> > mill(100, vector<int>(100));
void putMill()
{
	int minVal = 2147000000;
	for (int i = 0; i < N; i++)
		minVal = (minVal > mill[N - 1][i]) ? mill[N - 1][i] : minVal;
	for (int i = 0; i < N; i++)
	{
		if (mill[N - 1][i] == minVal) mill[N - 1][i]++;
	}
}
void test_printMill()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << mill[i][j] << "\t";
		cout << "\n";
	}
}
void rollMill()
{
	int rSize = 1;
	int cSize = 1;
	int startCol = 0;
	while (1)
	{
		// 바닥보다 길어지면 중단
		if (rSize > N - 1 - (startCol + cSize) + 1)
		{
			break;
		}

		// 회전해서 새로 저장 c가 r이되고 r이 c가 된다
		vector<vector<int> > temp(cSize, vector<int>(rSize));
		for (int i = 0; i < cSize; i++)
		{
			for (int j = 0; j < rSize; j++)
			{
				temp[i][j] = mill[N - 1 - j][startCol + i];
				mill[N - 1 - j][startCol + i] = 0;
			}
		}
		// 회전한것 원래 배열에 넣기
		for (int i = 0; i < cSize; i++)
		{
			for (int j = 0; j < rSize; j++)
			{
				mill[N - 1 - cSize + i][startCol + cSize + j] = temp[i][j];
			}
		}
		startCol += cSize;
		if (rSize == cSize) rSize++;
		else if (rSize > cSize) cSize++;
		// 회전 결과 출력테스트
		//cout << "[[[" << turn << "]]\n";
		//test_printMill();
	}
}
void linearMill()
{
	int temp[100] = { 0 };
	int idx = 0;
	for (int j = 0; j < N;j++)
	{
		for (int i = N - 1; i >= 0; i--)
		{
			if (mill[i][j] != 0)
				temp[idx++] = mill[i][j];
			else if (mill[i][j] == 0) break;
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			mill[i][j] = 0;
	}
	for (int i = 0; i < N; i++)
		mill[N - 1][i] = temp[i];
	//cout << "펼치기\n";
	//test_printMill();
}
void pressMill()
{
	vector<vector<int> > temp(N, vector<int>(N));
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (mill[i][j] != 0)
			{
				for (int k = 0; k < 4; k++)
				{
					int nRow = i + drow[k];
					int nCol = j + dcol[k];
					if (nRow >= N or nRow < 0 or nCol >= N or nCol < 0) continue;
					if (mill[nRow][nCol] == 0) continue;
					int offset = abs(mill[i][j] - mill[nRow][nCol]) / 5;
					if (mill[i][j] > mill[nRow][nCol])
					{
						temp[i][j] -= offset;
					}
					else
					{
						temp[i][j] += offset;
					}
				}
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			mill[i][j] += temp[i][j];
	}
	//cout << "press결과\n";
	//test_printMill();
	linearMill();
}
void foldTwice()
{
	for (int j = 0; j < N / 2; j++)
	{
		mill[N - 2][N - 1 - j] = mill[N - 1][j];
		mill[N - 1][j] = 0;
	}
	//cout << "한법접기\n";
	//test_printMill();
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < N / 4; j++)
		{
			mill[N - 4 + i][N - 1 - j] = mill[N - 1 - i][N / 2 + j];
			mill[N - 1 - i][N / 2 + j] = 0;
		}
	}
	//cout << "두법접기\n";
	//test_printMill();
}
int main()
{
	cin >> N >> K;
	for (int i = 0; i < N; i++)
		cin >> mill[N - 1][i];
	while (1)
	{
		int maxVal = -1;
		int minVal = 2147000000;
		for (int i = 0; i < N; i++)
		{
			maxVal = maxVal < mill[N - 1][i] ? mill[N - 1][i] : maxVal;
			minVal = minVal > mill[N - 1][i] ? mill[N - 1][i] : minVal;
		}
		if (abs(maxVal - minVal) <= K) break;
		else
		{
			//cout << minVal << " " << maxVal << "\n";
			putMill();
			rollMill();
			pressMill();
			foldTwice();
			pressMill();
			turn++;
		}
	}
	cout << turn;
}