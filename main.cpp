#include<iostream>
#include<ctime>
using namespace std;

int* GetRandNumber()
{
	srand((unsigned int)time(nullptr));
	bool visited[10] = { false, };
	int* tmp = new int[4];
	for (int i = 0; i < 4;)
	{
		*(tmp+i) = rand() % 10;
		if (!visited[*(tmp + i)])
		{
			visited[*(tmp + i)] = true;
			i++;
		}
	}
	return tmp;
}
int* GetPlayersNum()
{
	int* tmp = new int[4];
	cout << "각기 다른 숫자 4개를 입력하세요 : ";
	for (int i = 0; i < 4; i++)
	{
		int num = 0;
		cin >> num;
		*(tmp + i) = num;
	}

	return tmp;
}
bool GetResult(int* compu, int* player)
{
	int cntstrike = 0;
	int cntball = 0;
	int cntout = 4;
	for (int i = 0; i < 4; i++)
	{
		if (*(compu +i) == *(player+i))
			cntstrike++;
	}

	if (cntstrike == 4)
	{
		cout << " Strike : 4\n";
		return false;
	}

	for(int i=0;i<4;i++)
		for (int j = 0; j < 4; j++)
		{
			if (*(compu + i) == *(player + j))
			{
				cntball++;
				break;
			}
		}

	cntball -= cntstrike;
	cntout -= (cntball + cntstrike);
	if (cntball >= 0)
	{
		//cout << *(compu + 0) << " " <<*(compu + 1) << " " <<*(compu + 2) << " " <<*(compu + 3) << " "<<'\n';
		cout << " Strike :" << cntstrike << " Ball : " << cntball << " Out : " << cntout << '\n';
		return true;
	}
	else
	{
		//cout << *(compu + 0) << " " << *(compu + 1) << " " << *(compu + 2) << " " << *(compu + 3) << " " << '\n';
		cout << " Strike :" << cntstrike << " Ball : " << 0 << " Out : " << cntout << '\n';
		return true;
	}
}
int main()
{
	int* compu = new int[4];
	compu = GetRandNumber();
	int* player = new int[4];
	player = nullptr;
	bool IsRunning = true;

	while (IsRunning)
	{
		player = GetPlayersNum();
		IsRunning = GetResult(compu, player);
	}

	delete[] compu;
	delete[] player;
	return 0;
}