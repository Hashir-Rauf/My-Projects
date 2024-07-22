#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>

using namespace std;

int ROWS = 0;
int COLS = 0; 
int board[100][100];
int limit = 0;
string player_names[2];
string tempSave = "D:\\temp_game.txt";

void getDimensions(int &ROWS)
{
    cout << "Enter the Number of Rows" << endl;
    cin >> ROWS;
    cout << "Enter the Number of Coloumns" << endl;
    cin >> COLS;
    cout << "Enter the connectivity limit :" << endl;
    cin >> limit;
    cin.ignore();
}

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
	system("cls");
	cout << endl << endl;
    for (int i = 0; i < COLS; i++)
    {
        cout << "----";
    }
    cout << "---";
    cout << endl;
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
    cout << "---";
    cout << endl;
    for (int i = 0; i < ROWS; i++)
    {
        cout << i << " | ";
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
void get_player_names()
{
    cout << "Enter name for Player 1: ";
    getline(cin, player_names[0]);

    cout << "Enter name for Player 2: ";
    getline(cin, player_names[1]);
}
int menu()
{
    int choice;
    cout << "1. Play\n"
         << "2. Save Game\n"
         << "3. Load Game\n"
         << "4. Instructions\n"
         << "5. Exit\n"
         << "Enter Your Choice (1 - 5): ";
    cin >> choice;
    return choice;
}

// To Clarify and clear the confusion of the players about what the game is about.
void Instructions()
{
	cout << endl << endl << endl 
		 << "|=====================================================================================================================|" << endl
	 	 << "|   1: Connect N is the modified version of Connect 4  .                                                              |" << endl
		 << "|   2: Connectivity Limit refers to the number of Connects required by one of the players to win the game             |"  << endl
		 << "|   3: The number of rows and coloums refers to the dimensions of the Connect N board  .                              | " << endl
		 << "|=====================================================================================================================|" << endl << endl << endl;
		 
}
// Implement the game loop
void save_game(string &filename)
{
    ofstream outfile(filename);

    // Save ROWS, COLS, limit, player names, and the board state
    outfile << ROWS << " " << COLS << " " << limit << endl;
    outfile << player_names[0] << endl;
    outfile << player_names[1] << endl;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            outfile << board[i][j] << " ";
        }
        outfile << endl;
    }

    outfile.close();

    cout << "Game saved to " << filename << endl;
}

// Load the game from a file
void load_game(string &filename)
{
    ifstream infile(filename);

    if (!infile)
    {
        cout << "Error: Could not open the file " << filename << endl;
        return;
    }

    // Load ROWS, COLS, limit, player names, and the board state
    infile >> ROWS >> COLS >> limit;
    infile.ignore();

    getline(infile, player_names[0]);
    getline(infile, player_names[1]);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            infile >> board[i][j];
        }
    }

    infile.close();

    cout << "Game loaded from " << filename << endl;
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

// Switch between players
int switch_player(int player)
{
    return (player == 1) ? 2 : 1;
}


void play_game()
{
    // If ROWS is not initialized, get dimensions from the user
    if (ROWS == 0)
    {
        getDimensions(ROWS);
        initialize_board();
        get_player_names();
    }

    int player = 1;
    int col;
    bool is_game_over = false;
    cout << endl
         << "Enter a Negative Number to return to the main menu" << endl;

    while (!is_game_over)
    {
        print_board();
        cout << endl
             << player_names[player - 1] << ",\a\a enter your move (select column): ";
        cin >> col;

        // Check if the entered column is negative
        if (col < 0)
        {
            cout << endl
                 << "Saving game and returning to the Main Menu\n";

            // Save the game state to a temporary file
            save_game(tempSave);

            // Clear input buffer
            cin.ignore();
            return; // Exit the function and return to the main menu
        }

        // Clear input buffer
        cin.ignore();

        if (is_move_valid(col))
        {
            place_move(player, col);

            if (check_win(player))
            {
                print_board();
                cout << player_names[player - 1] << " wins!\n";
                is_game_over = true;
            }
            else if (check_draw())
            {
                print_board();
                cout << "It's a draw!\n";
                is_game_over = true;
            }
            else
            {
                player = switch_player(player);
            }
        }
        else
        {
            cout << "Invalid move. Try again.\n";
        }
    }
}
int main()

{
    int Choice;
    string filelocation;

    cout << "Hello, Welcome to Connect N, If you are new then check instructions first." << endl;
    
    do
    {
        Choice = menu();
        
        switch (Choice)
        {
            case 1:
                play_game();
                break;
                
            case 2:
                cout << "Enter the Location where you want to save the game: ";
                cin >> filelocation;
                save_game(filelocation);
                break;
                
            case 3:
                cout << "Enter the Location from where you want to load the game: ";
                cin >> filelocation;
                load_game(filelocation);
                break;
                
            case 4:
                Instructions();
                break;
        }
    } while (Choice > 0 && Choice < 5);


    return 0;
}

