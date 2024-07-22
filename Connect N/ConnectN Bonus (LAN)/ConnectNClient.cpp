#include "ConnectNCommon.h"
#include "SocketIO.h"

server_info sinfo;
SOCKET server_socket;

int initialize()
{
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

	cout << ">> Connecting to Server " << sinfo.ip << ":" << sinfo.port << " ..." << endl;

  	struct hostent* host = gethostbyname(sinfo.ip.c_str());
  	
  	sockaddr_in serverSockAddr;
    serverSockAddr.sin_family = AF_INET; 
    serverSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    serverSockAddr.sin_port = htons(sinfo.port);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
   //try to connect...
    int status = connect(server_socket, (sockaddr*) &serverSockAddr, sizeof(serverSockAddr));
    if(status < 0)
    {
        print_wsa_error("Error connecting to socket!");
        return 3;
    }
    print_line("Connected to the server!", FILLER_LEN);

	return 0;
}

int get_initial_state()
{
	char read_buffer[1025];
	int bytes = receive_message(server_socket, read_buffer, 1024);
	//Expected data is in the form: ROWS,COLS,LIMIT,PLAYER TO MAKE THE MOVE,Player 1 Name
	vector<string> params = split_string(read_buffer, ',');

	ROWS = atoi(params.at(0).c_str());
	COLS = atoi(params.at(1).c_str());
	limit = atoi(params.at(2).c_str());
	int player = atoi(params.at(3).c_str());
	player_names[0] = params.at(4);

	cout
		<< endl
		<< "Player '" << player_names[0] << "' has initialized game with following values:" << endl
		<< "Rows=" << ROWS << endl
		<< "Cols=" << COLS << endl
		<< "Limit=" << limit << endl;
	if (player == 2)
	{
		cout << "You will make first move" << endl;
	}
	else
	{
		cout << player_names[0]  << " will make first move" << endl;
	}
	
	return player;
}	

void play_networked_game(int player)
{
	initialize_board();
	
	int col;
    bool is_game_over = false;

    while (!is_game_over)
    {
        print_board();
        if (player == 1)
        {
	        col = get_network_player_move(player, server_socket);
		}
		else
		{
	        col = get_player_move(player);
		}

        // Check if the entered column is negative
        if (col < 0)
        {
        	if (player == 2)
        	{
	        	send_int(server_socket, col);
			}
            return; // Exit the function and return to the main menu
        }

        if (is_move_valid(col))
        {
        	if (player == 2)
        	{
	        	send_int(server_socket, col);
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
}

int send_player_name()
{
	return send_message(server_socket, player_names[1].c_str());
}

int main()
{
	system("cls");
	instructions();

	get_player_name("[Player 2] Enter your name: ", 2);
	int status = initialize();
	if ( status != 0)
	{
		return status;
	}
	int player = get_initial_state();

	send_player_name();

	play_networked_game(player);
	return 0;
}
