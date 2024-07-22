/*
	ConnectNCommon.h
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	//For stringstream
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <winsock2.h>

//#define __DISABLE_PRINT__

#define FILLER_LEN 3

using namespace std;

// Global Variables
int ROWS = 0;
int COLS = 0; 
int board[100][100];
int limit = 0;
bool is_networked = false;
string player_names[2] = {"Player 1", "Player 2"};

// To Clarify and clear the confusion of the players about what the game is about.
void instructions()
{
	cout
		<< endl 
		<< "|=======================================================================================================|" << endl
	 	<< "| 1: Connect N is the modified version of Connect 4  .                                                  |" << endl
		<< "| 2: Connectivity Limit refers to the number of Connects required by one of the players to win the game |" << endl
		<< "| 3: The number of rows and coloums refers to the dimensions of the Connect N board  .                  |" << endl
		<< "|=======================================================================================================|" << endl 
		<< endl;
}

/**/
string num_to_str(int num)
{
	char temp_buff[16];
	itoa(num, temp_buff, 10);
	string temp_str(temp_buff);
	return temp_str;
}


// Update the check_win function for Connect N
bool check_win(int player)
{
    // Check columns
    for (int j = 0; j < COLS; j++)
    {
        int count = 0;
        for (int i = 0; i < ROWS; i++)
        {
            if (board[i][j] == player)
            {
                count++;
                if (count == limit)
                    return true;
            }
            else
            {
                count = 0;
            }
        }
    }

    // Check rows
    for (int i = 0; i < ROWS; i++)
    {
        int count = 0;
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == player)
            {
                count++;
                if (count == limit)
                    return true;
            }
            else
            {
                count = 0;
            }
        }
    }

    // Check diagonals
    for (int i = 0; i < ROWS - limit + 1; i++)
    {
        for (int j = 0; j < COLS - limit + 1; j++)
        {
            // Check diagonally (bottom-left to top-right)
            int count = 0;
            for (int k = 0; k < limit; k++)
            {
                if (board[i + k][j + k] == player)
                {
                    count++;
                    if (count == limit)
                        return true;
                }
                else
                {
                    count = 0;
                }
            }

            // Check diagonally (top-left to bottom-right)
            count = 0;
            for (int k = 0; k < limit; k++)
            {
                if (board[i + k][j + limit - 1 - k] == player)
                {
                    count++;
                    if (count == limit)
                        return true;
                }
                else
                {
                    count = 0;
                }
            }
        }
    }

    return false;
}
bool check_draw()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

void get_player_name(const char* prompt, int index)
{
	if (prompt == NULL)
	{
    	cout << "Enter NAME for Player " << index << ": ";
	}
	else
	{
		cout << prompt;
	}
    getline(cin, player_names[index - 1]);
}
void get_player_name(int index)
{
	get_player_name(NULL, index);
}

vector<string> split_string(const char* source, char delimiter)
{
    vector<string> sv;
    string str;

    stringstream ss(source); 
    while (getline(ss, str, delimiter)) 
    {
        sv.push_back(str);
	}

	return sv;
}

/*
*/
bool is_valid_choice(int choice, const char* valid_choices)
{
	while ( *valid_choices )
	{
		if ( choice == *valid_choices )
			return true;

		valid_choices ++;
	}
	return false;
}

/*
*/
void print_valid_choices(const char* prompt,const char* choices)
{
	int choice_count = strlen(choices);
	int i=0;
	cout << endl << prompt << "[";
	for(; i<choice_count - 1; i++)
	{
		cout << *(choices + i) << ", ";
	}
	cout << *(choices + i) << "]";
}

/*
*/
int get_choice(const char* prompt, const char* valid_choices)
{
	int choice;
	for(;;)
	{
		cout << prompt;
		choice = getche();
		if (is_valid_choice(choice, valid_choices))
			break;
		print_valid_choices("Invalid input. Valid choices are ", valid_choices);
		cout << "\x1b[A" << "\r";	//Move cursor one line up and at the start
	}
	cout << endl;
	return choice;
}

/****/
// Initialize the game board
void initialize_board()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = 0;
        }
    }
}
// Print the game board
void print_board()
{
	cout << endl << endl;
    for (int i = 0; i < COLS; i++)
    {
        cout << "----";
    }
    cout << "---" << endl;
    cout << "   ";
    for (int i = 0; i < COLS; i++)
    {
        cout << " " << i << "  ";
    }
    cout << endl;
    for (int i = 0; i < COLS; i++)
    {
        cout << "----";
    }
    cout << "---" << endl;
    for (int i = 0; i < ROWS; i++)
    {
        cout << (i<10 ? " " : "") << i << " | ";
        for (int j = 0; j < COLS; j++)
        {
        	if (board[i][j] == 0)
				cout << ".";
			else
			{
				if(board[i][j] == 1)
				{
					cout << "o";
				}
				else
				{
					cout << "x";
				}
			}
			//cout << board[i][j];
			cout  << " | ";
        }
        cout << "\n";
    }
    for (int i = 0; i < COLS; i++)
    {
        cout << "----";
    }
    cout << "---" << endl;
}

// Check if a move is valid
bool is_move_valid(int col)
{
    return col >= 0 && col < COLS && board[0][col] == 0; // Check the top row of the selected column
}

// Place a move on the game board
void place_move(int player, int col)
{
    for (int i = ROWS - 1; i >= 0; --i)
    {
        if (board[i][col] == 0)
        {
            board[i][col] = player;
            break;
        }
    }
}

void get_dimensions()
{
	cout << endl;
    cout << "Enter the Number of Rows: ";
    cin >> ROWS;
    cout << "Enter the Number of Coloumns: ";
    cin >> COLS;
    cout << "Enter the Connectivity limit: ";
    cin >> limit;
    cin.ignore();
}

int get_player_move(int player)
{
	int column;
	cout << endl << "[" << player_names[player - 1] << "] Select Column (-1 to Exit): ";
	cin >> column;
	cin.ignore();
	return column;
}

// Switch between players
int switch_player(int player)
{
    return (player == 1) ? 2 : 1;
}
