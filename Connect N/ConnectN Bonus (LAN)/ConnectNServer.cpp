#include "ConnectNCommon.h"
#include "SocketIO.h"

server_info sinfo;
SOCKET server_socket;
SOCKET client_socket;

string tempSave = "D:\\temp_game.txt";

void get_player_names()
{
	get_player_name(1);
	if (!is_networked)
	{
		get_player_name(2);
	}
}

int toss_for_first_move()
{
	cout << endl << "Press ENTER to TOSS for First Move ...";
	while (getch() != 13);
	srand(time(NULL));
	int player = (rand() % 2) + 1;
	
	cout << endl << player_names[player-1] << " wins and will move first" << endl << endl;
	
	return player;
}

int initialize_game()
{
    get_dimensions();
    initialize_board();
	get_player_names();

    int player = toss_for_first_move();
    return player;
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

void play_game()
{
    int col;
    bool is_game_over = false;

    int player = initialize_game();

    while (!is_game_over)
    {
        print_board();
        col = get_player_move(player);

        // Check if the entered column is negative
        if (col < 0)
        {
            cout << endl << "Saving game and returning to the Main Menu\n";

            // Save the game state to a temporary file
            save_game(tempSave);
            return; // Exit the function and return to the main menu
        }

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

int game_mode_menu()
{
	cout
		<< "** Connect N - Game Mode Menu **" << endl
		<< "--------------------------------" << endl
		<< ":  1. Start Local Game         :" << endl
		<< ":  2. Start Network Game       :" << endl
		<< ":  0. Exit                     :" << endl
		<< "--------------------------------" << endl
	;
	int choice = get_choice("Enter your choice: ", "120");
	return choice;
}
int menu()
{
    cout
    	<< endl
		<< "** Connect N - Game Menu **" << endl
		<< "---------------------------" << endl
		<< ":  1. Play                :" << endl
        << ":  2. Save Game           :" << endl
        << ":  3. Load Game           :" << endl
        << ":  4. Instructions        :" << endl
        << ":  0. Exit                :" << endl
		<< "---------------------------" << endl;
    int choice = get_choice("Enter your choice: ", "12340");
    return choice;
}

void start_local_game()
{
    int Choice;
    string filelocation;

    cout << endl << endl << "Hello, Welcome to Connect N, If you are new then check instructions first." << endl;
    do
    {
        Choice = menu();
        
        switch (Choice)
        {
            case '1':
                play_game();
                break;
                
            case '2':
                cout << "Enter the Location where you want to save the game: ";
                cin >> filelocation;
                save_game(filelocation);
                break;
                
            case '3':
                cout << "Enter the Location from where you want to load the game: ";
                cin >> filelocation;
                load_game(filelocation);
                break;
                
            case '4':
                instructions();
                break;
        }
    } while (Choice > '0' && Choice < '5');
}

void send_initial_state(SOCKET socket, int player)
{
		string str_init = 
			num_to_str(ROWS) + "," + 
			num_to_str(COLS) + "," + 
			num_to_str(limit) + "," + 
			num_to_str(player) + "," + 
			player_names[0];
	send_message(socket, str_init.c_str());
}

void play_networked_game(int player)
{
    int col;
    bool is_game_over = false;

    while (!is_game_over)
    {
        print_board();
        if (player == 1)
        {
	        col = get_player_move(player);
		}
		else
		{
	        col = get_network_player_move(player, client_socket);
		}
        // Check if the entered column is negative
        if (col < 0)
        {
        	if (player == 1)
        	{
	        	send_int(client_socket, col);
			}
            return; // Exit the function and return to the main menu
        }

        if (is_move_valid(col))
        {
        	if (player == 1)
        	{
	        	send_int(client_socket, col);
			}
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
    getch();
}

int get_network_player_name(int player)
{
	char player_name[1025];
	int bytes_read = receive_message(client_socket, player_name, 1024);
	if (bytes_read > 0)
	{
		player_names[player - 1] = player_name;
	}
	return bytes_read;
}

int start_networked_game()
{
	is_networked = true;
	
	sinfo = get_server_info();
	if (!sinfo.is_valid)
	{
		print_line("Game Server Information could not be loaded.");
		return 1;
	}
	if (!initialize_socket())
	{
		print_line("Windows Socket could not be initialized.");
		return 2;
	}
	
	server_socket = bind_server(sinfo.port);
	if(server_socket == SOCKET_ERROR)
	{
		return 3;
	}
	
	int player = initialize_game();

	client_socket = get_client(server_socket, "Waiting for Player 2 to Connect ...");
	if (client_socket == SOCKET_ERROR)
	{
		return 4;
	}
	
	send_initial_state(client_socket, player);
	
	if (get_network_player_name(2) > 0)
	{
		cout << endl << player_names[1] << " Connected" << endl;
		play_networked_game(player);
	}
	
	return 0;
}

int main()
{
	system("cls");
	instructions();
	
	int game_mode = game_mode_menu();
	
	if (game_mode == '1')
	{
		start_local_game();
	}
	else if (game_mode == '2')
	{
		start_networked_game();
	}
	return 0;
}
