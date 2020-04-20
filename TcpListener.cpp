#include "TcpListener.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int TcpListener::init()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}

	// Create a socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	// Tell Winsock the socket is for listening 
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}


	listen(m_socket, SOMAXCONN);


	
	
	return 0;
}

int TcpListener::run()
{

	int clientSize = sizeof(m_client);
	m_clientsocket = accept(m_socket, (sockaddr*)&m_client, &clientSize);
	if (m_clientsocket == INVALID_SOCKET)
	{
		cerr << "Can't create client socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&m_client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &m_client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(m_client.sin_port) << endl;
	}

	//send(m_clientsocket, "Hello, world!\n", 13, 0);
	// this will be changed by the \quit command
	bool running = true;

	char buf[4096];


	while (running)
	{
		ZeroMemory(buf, 4096);
		int bytesRES = recv(m_clientsocket, buf, 4096, 0);

		if (bytesRES == SOCKET_ERROR)
		{
			cerr << "ERROR in rec.... QUITTING" << endl; 
			break;
		}
		if (bytesRES == 0)
		{
			cout << "Disconnected" << endl;
			break;
		}
		//ECHO MSG
		
		//
		//onMessageReceived(m_clientsocket, buf, 4096);


		send(m_clientsocket, buf, 4096, 0);

	}
	//onMessageReceived(m_clientsocket, buf, 4096);


	// Cleanup winsock
	WSACleanup();
	return 0;
}


void TcpListener::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length)
{

}

void TcpListener::onClientConnected(int clientSocket)
{

}

void TcpListener::onClientDisconnected(int clientSocket)
{

}

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length)
{
	cout << msg << endl;


	// Write the document back to the client
	std::ostringstream oss;
	oss << "HTTP/1.1 " <<  " OK\r\n";
	oss << "\r\n";

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}
