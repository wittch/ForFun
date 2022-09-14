#include<iostream>
#include<conio.h>
#include<windows.h>

#define INF 9999

using namespace std;

bool IsRunning = true;
int player_x = 1;
int player_y = 1;
char prev_key = NULL;
bool Is_Food_Exist = false;
int Snake_length = 5;
int snake_food = -1;
int keyCode;
int Map[102][102];


void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

pair<int, int> makeMap()
{
	int x = -1;
	int y = -1;
	bool flg = true;
	while (flg) {
		cout << "값을 입력해 주세요. (10~100) \n";
		cin >> x >> y;

		if (x > 100 || y > 100 || x < 9 || y < 9) continue;

		else {
			for (int i = 0; i < x; i++) {
				Map[0][i] = INF;
				Map[y - 1][i] = INF;
			}
			for (int i = 0; i < y; i++) {
				Map[i][0] = INF;
				Map[i][x - 1] = INF;
			}

			flg = false;
		}
	}
	return { x,y };
}

int Input()
{
	//prevkey를 통해 이전에 왔던 방향으로 돌아가지 못하게 함
	if (keyCode != NULL)
		prev_key = keyCode;
	keyCode = _getch();
	return keyCode;
}

void Tick(int x, int y)
{

	if (keyCode == 'Q' || keyCode == 'q')
	{
		IsRunning = false;
		return;
	}

	if (!Is_Food_Exist && Snake_length < (x - 2) * (y - 2)) {
		bool flg = true;
		while (flg)
		{
			snake_food = rand() % ((x - 2) * (y - 2)) + x - 2;
			if (Map[snake_food / (x - 2)][snake_food % (x - 2)] != 0) continue;
			else flg = false;
		}
		Map[snake_food / (x - 2)][snake_food % (x - 2)] = 70;
		Is_Food_Exist = true;
	}

	//방향키 w,a,s,d
	switch (keyCode)
	{
	case 'w':
		if (prev_key != 's') {
			if (player_y > 1) player_y--;
			break;
		}
		else
		{
			keyCode = NULL;
		}
	case 's':
		if (prev_key != 'w') {
			if (player_y < (y - 2)) player_y++;
			break;
		}
		else
		{
			keyCode = NULL;
		}
	case 'a':
		if (prev_key != 'd') {
			if (player_x > 1) player_x--;
			break;
		}
		else
		{
			keyCode = NULL;
		}
	case 'd':
		if (prev_key != 'a')
		{
			if (player_x < (x - 2)) player_x++;
			break;
		}
		else
		{
			keyCode = NULL;
		}
	default:
		break;
	}
	//뱀이 자신이나 벽에 닿으면 종료
	if (Map[player_y][player_x] < 70 && Map[player_y][player_x] > 0) IsRunning = false;
	//뱀이 가득 차면종료
	if (Snake_length == (x-2)*(y-2))
	{
		IsRunning = false;
	}
}
void Draw(int x, int y)
{


	system("cls");

	if (player_y == snake_food / (x - 2) && player_x == snake_food % (x - 2)) {
		Is_Food_Exist = false;
		Snake_length++;
	}
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			if (player_x == j && player_y == i)
				Map[i][j] = Snake_length;

			if (Map[i][j] > 0 && Map[i][j] < 69)
				Map[i][j]--;

			if (Map[i][j] == 0) printf_s(" ");//cout << ' ';
			else if (Map[i][j] == INF) printf_s("o");//cout << 'o';
			else if (Map[i][j] == 70) printf_s("F");//cout << 'F';
			else if (player_x == j && player_y == i)
				printf_s("h");// 머리 출력
			else
				printf_s("s");// 몸통 출력
		}
		printf_s("\n");//cout << '\n';
	}
}
int main()
{


	srand((unsigned int)time(NULL));
	pair<int, int> coord = makeMap();



	CursorView(false);

	while (IsRunning)
	{

		Input();
		Tick(coord.first, coord.second);
		Draw(coord.first, coord.second);

	}
}