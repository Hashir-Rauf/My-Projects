
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
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 29
#define MAX_STARS 250
#define SPRITE_WIDTH 45
#define SPRITE_HEIGHT 7
#define UPDATE_DELAY 25

#define KEY_ESCAPE 27
#define KEY_ARROW_SCAN_CODE 224
#define KEY_UP (KEY_ARROW_SCAN_CODE + 72)
#define KEY_DOWN (KEY_ARROW_SCAN_CODE + 80)
#define KEY_LEFT (KEY_ARROW_SCAN_CODE + 75)
#define KEY_RIGHT (KEY_ARROW_SCAN_CODE + 77)

using namespace std;

const char fighterSprite[SPRITE_HEIGHT][SPRITE_WIDTH] =
{
"  ___",
" |   \\",
" |    \\                   ___",
" |_____\\______________.-'`   `'-.,___",
"/| _____     _________            ___>---",
"\\|___________________________,.-'`",
"          `'-.,__________)",
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
		if (key == KEY_ARROW_SCAN_CODE)
		{
			key += getch();
		}
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
		for(int j=0; j<SCREEN_WIDTH; j++)
		{
			screenBuffer[i][j] = starsBuffer[i][j];
		}
	}
}
void drawFighter(int x, int y)
{
	for(int	i=0; i<SPRITE_HEIGHT; i++)
	{
		for(int j=0; fighterSprite[i][j]; j++)
		screenBuffer[y+i][x+j] = fighterSprite[i][j];
	}
}
void prepareScene(int fighterPosX, int fighterPosY)
{
	drawStars();
	drawFighter(fighterPosX, fighterPosY);
}
void drawScene()
{
	for(int i=0; i<SCREEN_HEIGHT; i++)
	{
		setCursorPosition(i, 0);
		cout << screenBuffer[i];
	}
}
char nextStar(int row)
{
	char star = ' ';
	if (random(SCREEN_HEIGHT) == row)
	{
		star = getStar();
	}
	return star;
}
void moveStars()
{
	for(int i=0; i<SCREEN_HEIGHT; i++)
	{
		for(int j=1; j<SCREEN_WIDTH; j++)
		{
			starsBuffer[i][j - 1] = starsBuffer[i][j];
		}
		starsBuffer[i][SCREEN_WIDTH-1] = nextStar(i);
	}
}
void updateScene()
{
	moveStars();
}
void moveFighter(int key, int &fighterPosX, int &fighterPosY)
{
	switch(key)
	{
		case KEY_UP:
			fighterPosY -= 1; 
			if (fighterPosY < 0) 
				fighterPosY = 0; 
			break;
		case KEY_DOWN:
			fighterPosY += 1; 
			if (fighterPosY > (SCREEN_HEIGHT-SPRITE_HEIGHT)) 
				fighterPosY = (SCREEN_HEIGHT-SPRITE_HEIGHT); 
			break;
		case KEY_LEFT:
			fighterPosX -= 3; 
			if (fighterPosX < 0) 
				fighterPosX = 0; 
			break;
		case KEY_RIGHT:
			fighterPosX += 3; 
			if (fighterPosX > (SCREEN_WIDTH-SPRITE_WIDTH)) 
				fighterPosX = (SCREEN_WIDTH-SPRITE_WIDTH); 
			break;
	}
}
int gameLoop()
{
	//Start at the center of screen
	int fighterPosX = (SCREEN_WIDTH - SPRITE_WIDTH) / 2;
	int fighterPosY = ((SCREEN_HEIGHT - SPRITE_HEIGHT) / 2) - 1; 
	showConsoleCursor(false);
	while (true)
	{
		prepareScene(fighterPosX, fighterPosY);
		drawScene();
		int key = getKeyPressed();
		if (key == KEY_ESCAPE) break;
		moveFighter(key, fighterPosX, fighterPosY);
		updateScene();
		delay(UPDATE_DELAY);
	}
	showConsoleCursor(true);
	return 0;
}

void testKeys()
{
	int key = 0;
	cout << "Press a key for its number, ESCAPE to exit :" << endl;
	while (key != KEY_ESCAPE)
	{
		key = getKeyPressed();
		if (key != 0)
			cout << "Key Pressed = " << key << endl;
	}
}
int main()
{
	initialize();
	gameLoop();
	//testKeys();
}
