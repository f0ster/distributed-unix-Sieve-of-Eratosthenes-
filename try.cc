#include <arpa/inet.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

#include <stdint.h>

#define MAX_MSG 100
#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_MSG+1)

using namespace std;





int server(unsigned short int listenPort, char buf[LINE_ARRAY_SIZE], unsigned short int serverPort);
int client(unsigned short int listenPort, char buf[LINE_ARRAY_SIZE], unsigned short int serverPort);



int main()
{

unsigned short int listenPort;
char buf[LINE_ARRAY_SIZE];
unsigned short int serverPort;


int isMain = 0;
cout << "Is this the Main application? " << endl;
cin >> isMain;
cout << "Enter port number to listen on (between 1500 and 65000): ";
cin >> listenPort;
cout << "Enter server host name or IP address: ";
cin >> buf;
cout << "Enter server port number: ";
cin >> serverPort;

if (isMain)
server(listenPort,buf,serverPort);
else
client(listenPort,buf,serverPort);


}

int server(unsigned short int listenPort, char buf[LINE_ARRAY_SIZE], unsigned short int serverPort){
 int connectSocket, listenSocket, socketDescriptor ;
  int size = 10;
  int *somedata = (int *) malloc(size * sizeof(int));
  for(int i=0; i<size; i++) {
	somedata[i] = i;
  }
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, thisAddress, serverAddress;
  struct hostent *hostInfo;
  bool notdone = true;
  
  // Create socket for listening for client connection requests.
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    cerr << "cannot create listen socket";
    exit(1);
  }
  
  thisAddress.sin_family = AF_INET;
  thisAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  thisAddress.sin_port = htons(listenPort);
  if (bind(listenSocket,
           (struct sockaddr *) &thisAddress,
           sizeof(thisAddress)) < 0) {
    cerr << "cannot bind socket";
    exit(1);
  }

  listen(listenSocket, 5);
  
  while (notdone) {
		cout << "Waiting for TCP connection on port " << listenPort << " ...\n";

		clientAddressLength = sizeof(clientAddress);
		connectSocket = accept(listenSocket,(struct sockaddr *) &clientAddress, &clientAddressLength);
		if (connectSocket < 0) {
		  cerr << "cannot accept connection ";
		  exit(1);
		}

		cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);
		cout << ":" << ntohs(clientAddress.sin_port) << "\n";
		//int size = 10	;
		//int *intarray = (int *) malloc(size * sizeof(int));
			uint32_t data;
			//int u=0;
			while(recv(connectSocket, &data, sizeof(data), 0) > 0){	
				cout << "received " << ntohl(data) << endl;
				//u++;
			}
			//cout << u << endl;

		// math


		hostInfo = gethostbyname(buf);
		if (hostInfo == NULL) {
		cout << "problem interpreting host: " << buf << "\n";
		exit(1);
		}

		socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		if (socketDescriptor < 0) {
		cerr << "cannot create socket\n";
		exit(1);
		}

		serverAddress.sin_family = hostInfo->h_addrtype;
		memcpy((char *) &serverAddress.sin_addr.s_addr,
			 hostInfo->h_addr_list[0], hostInfo->h_length);
		serverAddress.sin_port = htons(serverPort);
					
		if (connect(socketDescriptor,
				  (struct sockaddr *) &serverAddress,
				  sizeof(serverAddress)) < 0) {
		cerr << "cannot connect\n";
		exit(1);
		}

		cout << "Sending some data..."<< endl;
		// Stop when the user inputs a line with just a dot.
		//while (strcmp(buf, ".")) {\

		for(int k=0; k < size; k++){
			uint32_t thelong;
			// Send the line to the server.
				//memcpy( buf, htonl(somedata[i]), 256*sizeof(char));
			//cout << "attemping to send " << somedata[k] << " as " << buf << endl;
			thelong = htonl( somedata[k] ) ;
			if (send(socketDescriptor, &thelong, sizeof(thelong), 0) < 0) {
			  cerr << "cannot send data ";
			  close(socketDescriptor);
			  exit(1);
			} 
		}	

		close(socketDescriptor);	
	
	}
	
}


int client(unsigned short int listenPort, char buf[LINE_ARRAY_SIZE], unsigned short int serverPort){
 int connectSocket, listenSocket, socketDescriptor ;
  int size = 10;
  int *somedata = (int *) malloc(size * sizeof(int));
  for(int i=0; i<size; i++) {
	somedata[i] = i;
  }
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, thisAddress, serverAddress;
  struct hostent *hostInfo;
  bool notdone = true,  firstTime = true;

  // Create socket for listening for client connection requests.
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    cerr << "cannot create listen socket";
    exit(1);
  }

  thisAddress.sin_family = AF_INET;
  thisAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  thisAddress.sin_port = htons(listenPort);
  if (bind(listenSocket,  (struct sockaddr *) &thisAddress,  sizeof(thisAddress)) < 0) {
    cerr << "cannot bind socket";
    exit(1);
  }


  listen(listenSocket, 5);
  
  while (notdone) {
  if (firstTime)
	goto here;
	cout << "Waiting for TCP connection on port " << listenPort << " ...\n";
	clientAddressLength = sizeof(clientAddress);
	connectSocket = accept(listenSocket,(struct sockaddr *) &clientAddress, &clientAddressLength);
	if (connectSocket < 0) {
	  cerr << "cannot accept connection ";
	  exit(1);
	}

	cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);
	cout << ":" << ntohs(clientAddress.sin_port) << "\n";
	//int size = 10	;
	//int *intarray = (int *) malloc(size * sizeof(int));
		uint32_t data;
		//int u=0;
		while(recv(connectSocket, &data, sizeof(data), 0) > 0){	
			cout << "received " << ntohl(data) << endl;
			//u++;
		}
		//cout << u << endl;

here:

	hostInfo = gethostbyname(buf);
	if (hostInfo == NULL) {
	cout << "problem interpreting host: " << buf << "\n";
	exit(1);
	}
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) {
	cerr << "cannot create socket\n";
	exit(1);
	}

	serverAddress.sin_family = hostInfo->h_addrtype;
	memcpy((char *) &serverAddress.sin_addr.s_addr,
		 hostInfo->h_addr_list[0], hostInfo->h_length);
	serverAddress.sin_port = htons(serverPort);
				
	if (connect(socketDescriptor,
			  (struct sockaddr *) &serverAddress,
			  sizeof(serverAddress)) < 0) {
	cerr << "cannot connect\n";
	exit(1);
	}

	cout << "Sending data..."<< endl;
	
	for(int k=0; k < size; k++){
		uint32_t thelong;
		thelong = htonl( somedata[k] ) ;
		if (send(socketDescriptor, &thelong, sizeof(thelong), 0) < 0) {
		  cerr << "cannot send data ";
		  close(socketDescriptor);
		  exit(1);
		} 
	}
	close(socketDescriptor);
	firstTime = false;
	}
	
}


