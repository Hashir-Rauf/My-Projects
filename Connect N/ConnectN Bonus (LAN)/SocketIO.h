
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <winsock2.h>

#ifndef FILLER_LEN
#define FILLER_LEN 3
#endif

#define __DISABLE_PRINT__

using namespace std;

//Global Structures Definitions
struct server_info
{
	string ip;
	int port;
	bool is_valid = false;
};

void _print_line(const char* message, int fillerLen, bool newLine)
{
#ifndef __DISABLE_PRINT__
	for(int i=0; i<fillerLen; i++)
		cout << " ";
	cout << message;
	if (newLine) cout << endl;
#endif	
}
void print(const char* message, int fillerLen)
{
	_print_line(message, fillerLen, false);
}
void print_line(const char* message, int fillerLen)
{
	_print_line(message, fillerLen, true);
}
void print_line(const char* message)
{
	print_line(message, 0);
}
void print_wsa_error(const char* message)
{
	cout << "***** " << message << " Error Code : " << WSAGetLastError() << " *****" << endl;
}

server_info get_server_info()
{
	server_info sinfo;
	
	const char* file_name = "game_server.txt";
	ifstream file(file_name);
	if (file)
	{
		file >> sinfo.ip >> sinfo.port;
		file.close();
		sinfo.is_valid = true;
	}
	return sinfo;
}

void close_socket(SOCKET socket)
{
	close_socket(socket);
	WSACleanup();
}
void close_socket_with_error(SOCKET socket, const char* message)
{
	print_wsa_error(message);
	close_socket(socket);
}

bool initialize_socket()
{
	//Step 1. Initialize Winsock
	WSADATA wsa;
	print_line(">> Initializing Winsock . . .", 0);
	int iResult = WSAStartup(0x202, &wsa);
	if (iResult != 0)
	{
		print_wsa_error("Winsock Initialization Failed.");
		return false;
	}
	print_line("Winsock Inititalized.", FILLER_LEN);
    return true;
}

int send_message(SOCKET socket, const char* message)
{
	char msg_buffer[10240];
	memset(&msg_buffer, 0, sizeof(msg_buffer));
	strcpy(msg_buffer, message);
  	int bytes_sent = send(socket, (char*)&msg_buffer, strlen(msg_buffer), 0);
  	return bytes_sent;
}
int send_int(SOCKET socket, int n)
{
	char msg_buffer[1025];
	memset(&msg_buffer, 0, sizeof(msg_buffer));
	strcpy(msg_buffer, num_to_str(n).c_str());
  	int bytes_sent = send(socket, (char*)&msg_buffer, strlen(msg_buffer), 0);
  	return bytes_sent;
}

int receive_message(SOCKET socket, char* buffer, int buffer_size)
{
	char msg_buffer[10240];
	memset(&msg_buffer, 0, sizeof(msg_buffer));
  	int bytes_read = recv(socket, (char *)&msg_buffer, buffer_size, 0);
  	strcpy(buffer, msg_buffer);
  	return bytes_read;
}
int receive_int(SOCKET socket)
{
	char input_buffer[1025];
	int bytes_read = receive_message(socket, input_buffer, 1024);
	int num = atoi(input_buffer);
	
	return num;
}

SOCKET bind_server(int port)
{
	int iFamily = AF_INET;		//IPv4
	int iType = SOCK_STREAM;	//Connection oriented TCP protocol
	int iProtocol = 0;			//Undefined or IPPROTO_TCP , IPPROTO_UDP
	//Function socket() creates a socket and returns a socket descriptor which can be used in other network commands.

	print_line(">> Creating Socket . . .", 0);
	SOCKET sock = socket(iFamily, iType, iProtocol);
	if (sock == INVALID_SOCKET)
	{
		print_wsa_error("Create Socket Failed.");
		return SOCKET_ERROR;
	}
	print_line("Socket Created.", FILLER_LEN);

	//Step 3. Bind to localhost
	// Server details
	int serverPort = port;
	sockaddr_in server;
	server.sin_addr.s_addr = INADDR_ANY;	//IP address that is used when we don't want to bind a socket to any specific IP
	server.sin_family = iFamily;
	server.sin_port = htons( serverPort );

	//Bind the server
	print_line(">> Binding Socket  . . .", 0);
	int iResult = bind(sock, (sockaddr *) &server, sizeof(server));
	if (iResult == SOCKET_ERROR)
	{
		close_socket_with_error(sock, "Bind Failed.");
		return SOCKET_ERROR;
	}
	print_line("Bind Successful.", FILLER_LEN);
	//listen for up to 5 requests at a time
	listen(sock, 5);
	//receive a request from client using accept
	//we need a new address to connect with the client
	return sock;
}
SOCKET get_client(SOCKET server, const char* message)
{
	sockaddr_in clinet_info;
	int clinet_info_size = sizeof(clinet_info);
	//accept, create a new socket descriptor to handle the new connection with client
	if (message != NULL)
	{
		//print_line("Listening for new Connections (Press Ctrl+C to Exit) ...", FILLER_LEN);
		cout << message << endl;
	}
	SOCKET client_socket = accept(server, (sockaddr *)&clinet_info, &clinet_info_size);
	if(client_socket == SOCKET_ERROR)
	{
	    print_wsa_error("Error accepting request from client!");
	}
	else
	{
	    print_line(">> Connected to the client!", 0);
	}
	return client_socket;
}

int get_network_player_move(int player, SOCKET socket)
{
	int column;
	cout << endl << "[" << player_names[player - 1] << "] Select Column (-1 to Exit): ";
	column = receive_int(socket);
	cout << column << endl;
	return column;
}
