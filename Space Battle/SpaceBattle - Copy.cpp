
// Include required header files
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <cstdlib> //For srand()
#include <time.h>
//
#define FIGURE_HEIGHT 15
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30
#define MAX_STARS 250
#define SPRITE_HEIGHT 8

using namespace std;

const string fighterSprite[SPRITE_HEIGHT] =
{
"  ___",
" |   \\",
" |    \\                   ___",
" |_____\\______________.-'`   `'-.,___",
"/| _____     _________            ___>---",
"\\|___________________________,.-'`",
"          `'-.,__________)",
""
};

char screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
char starsBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void delay(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void setCursorPosition(int row, int col)
{
	HANDLE handle;
	COORD coordinates;
	
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = col;
	coordinates.Y = row;
	SetConsoleCursorPosition(handle, coordinates);
}

void showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

int getKeyPressed()
{
	int key = 0;
	int keyPressed = kbhit();
	if (keyPressed)
	{
		key = getch();
	}
	return key;
}
int random(int max)
{
	return (rand() % max);
}
void initializeBuffer(char buffer[], int size, char c)
{
	for(int i=0; i<SCREEN_WIDTH; i++)
	{
		buffer[i] = c;
	}
}
void initializeScreenBuffer()
{
	for(int i=0; i<SCREEN_HEIGHT; i++)
	{
		initializeBuffer(screenBuffer[i], SCREEN_WIDTH+1, '\0');
	}
}
void initializeStarsBuffer()
{
	for(int i=0; i<SCREEN_HEIGHT; i++)
	{
		initializeBuffer(starsBuffer[i], SCREEN_WIDTH, ' ');
	}
}

char getStar()
{
	return ('\'');
}
void initialize()
{
	srand(time(0)); //Initialize pseudo random number generator using current time

	initializeScreenBuffer();
	initializeStarsBuffer();
	for(int i=0; i<MAX_STARS; i++)
	{
		int row = random(SCREEN_HEIGHT);
		int col = random(SCREEN_WIDTH);
		starsBuffer[row][col] = getStar();
	}
}
void drawStars()
{
	for (int i=0; i<SCREEN_HEIGHT; i++)
	{
		setCursorPosition(i, 0);
		for(int j=0; j<SCREEN_WIDTH; j++)
		{
			cout << starsBuffer[i][j];
		}
	}
}
void drawFighter(int x, int y)
{
	for(int	i=0; i<SPRITE_HEIGHT; i++)
	{
		setCursorPosition(x, y);
		cout << fighterSprite[i];
		x++;
	}
}
void prepareScene(int fighterPosX, int fighterPosY)
{
	drawStars();
	drawFighter(fighterPosX, fighterPosY);
}
void drawScene()
{
}
int gameLoop()
{
	bool done = false;
	int fighterPosX = 0;
	int fighterPosY = 0;
	showConsoleCursor(false);
	while (!done)
	{
		prepareScene(fighterPosX, fighterPosY);
		drawScene();
		delay(125);
		int key = getKeyPressed();
		done = (key == 27);
	}
	showConsoleCursor(true);
	return done;
}

int main()
{
	initialize();
	gameLoop();
}
