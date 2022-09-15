#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<conio.h>
#include<windows.h>
#include<sstream>


#define INF 9999
#define BUF_SIZE 1023
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
static int g_nScreenIndex;
static HANDLE g_hScreen[2];
int g_numOfFrame;
int g_numOfFPS;
char* FPSTextInfo;
clock_t OldTime;
clock_t CurTime;
//ostringstream FPSTextInfo;

void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
void ScreenInit()
{
	CONSOLE_CURSOR_INFO cci;
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}
void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}
void ScreenClear()
{
	COORD Coor = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 40, Coor, &dw);
}
void ScreenPrint(int x, int y, char* string)
{
	DWORD dw;
	COORD CursorPosition = { x,y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}
void ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}
void Release()
{
	delete[] FPSTextInfo;
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
	ScreenClear();//system("cls");
	cout << '\n';
	cout << '\n';
	cout << '\n';
	cout << "          시작하려면 아무 키나 누르세요.  ";
	//system("mode con cols=20 lines=20"); 콘솔창 크기 변경
	return { x,y };
}



int Input()
{
	keyCode = _getch();
	return keyCode;
}
void Tick(int x, int y)
{

	if (keyCode != NULL)
		prev_key = keyCode;

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
	if (Map[player_y][player_x] < 70 && Map[player_y][player_x] > 0) IsRunning = false;
	if (Snake_length == (x - 2) * (y - 2) - 1)
	{
		IsRunning = false;
	}
}
void Draw(int x, int y)
{
	DWORD dw;
	string tmp;

	ScreenClear();

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

			if (Map[i][j] == 0) tmp.push_back(' ');
			else if (Map[i][j] == INF) tmp.push_back('o');
			else if (Map[i][j] == 70) tmp.push_back('F');
			else if (player_x == j && player_y == i)
				tmp.push_back('@');
			else
				tmp.push_back('*');
		}
		tmp.push_back('\n');
	}
	



	if (CurTime - OldTime >= 1000)
	{
		OldTime = CurTime;
		g_numOfFPS = g_numOfFrame;
		g_numOfFrame = 0;
	}


	strcpy(FPSTextInfo, tmp.c_str());
	ScreenPrint(0, 0, FPSTextInfo);


	ScreenFlipping();
}
int main()
{
	FPSTextInfo = new char[BUF_SIZE];
	memset(FPSTextInfo, '\0', BUF_SIZE);

	srand((unsigned int)time(NULL));
	pair<int, int> coord = makeMap();

	ScreenInit();
	OldTime = clock();


	//CursorView(false);

	while (IsRunning)
	{
		CurTime = clock();
		Input();
		Tick(coord.first, coord.second);
		Draw(coord.first, coord.second);

	}
	ScreenRelease();
	Release();

	return 0;
}