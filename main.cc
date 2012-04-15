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

unsigned short int listenPort;
char buf[LINE_ARRAY_SIZE];
unsigned short int serverPort;


  int connectSocket;
  unsigned short int  listenSocket;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, thisAddress;
  
int size = 0;


int clent();
int server();


//vector<int> primes;

int main()
{

int isMain = 0;
bool quit = false;
int prime = 2;
cout << "Is this the Main application? " << endl;
cin >> isMain;
cout << "Enter port number to listen on (between 1500 and 65000): ";
cin >> listenPort;
cout << "Enter server host name or IP address: ";
cin >> buf;
cout << "Enter server port number: ";
cin >> serverPort;
//if (!isMain){
cout << "Enter Upper Bound :" << endl;
cin >> size;
//}

int *list = (int *) malloc(size * sizeof(int));
	  for(int i=1; i<size; i++) {
		if(isMain)
			list[i] = 0;
		else
			list[i] = i;
	  }
	  
int *primes = (int *) malloc(size * sizeof(int));
	for(int i=1; i<size; i++) {
		primes[i] = 0;
	}

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
  cout << listenSocket << endl;
  if (bind(listenSocket,
           (struct sockaddr *) &thisAddress,
           sizeof(thisAddress)) < 0) {
    cerr << "cannot bind socket";
    exit(1);
  }

  listen(listenSocket, 5);
  


bool skip = false;
while(notdone){

//receive
	if (isMain == 1 || skip){

	    cout << "Waiting for TCP connection on port " << listenPort << " ...\n";
	    clientAddressLength = sizeof(clientAddress);
	    connectSocket = accept(listenSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
	    if (connectSocket < 0) {
	      cerr << "cannot accept connection ";
	      exit(1);
	    }
	    cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);
	    cout << ":" << ntohs(clientAddress.sin_port) << "\n";
		
		uint32_t data;
		int u=0;
		
		if(recv(connectSocket, &data, sizeof(data), 0) > 0)
			prime = ntohl(data);
			if (prime == -1){
				
				cout << "exit()" << endl;
				exit(0);
				}
		while(recv(connectSocket, &data, sizeof(data), 0) > 0){	
			cout << "received " << ntohl(data) << endl;
			list[u] = ntohl(data);
			u++;
		}
		cout << "prime " << prime << endl;
		//cin >> u;
		for(int i = 0 ; i < size ; i ++ ) {
		 if (primes[i] == 0){
			primes[i] = prime;
			goto skipper;
			}
			//cout << "Prime[i] : " << primes[i] << endl;
		}
		skipper:
		
		for(int i = 0 ; i < size ; i ++ ) {
			cout << "Prime[i] : " << primes[i] << endl;
		}
	
	}
	bool first = true;
	
	int temp = prime;
	for(int i=temp; i < size; i++)
		{
			//If the entry has been set to 0 ('removed'), skip it
			if(list[i] > 0 && first)
			{			
				first  = false;
				temp = list[i];
				prime = temp;
				cout << "this is the new temp" << temp << endl;
			}
			if (!first && list[i] == temp){
				temp += temp;	
				cout << "this is temp : " << temp << endl;
				list[i] = 0;
			}
		}
		if (first)
			quit = true;

	//	exit(0);
		
	  skip = true;
	  int socketDescriptor;
	  //unsigned short int serverPort;
	  struct sockaddr_in serverAddress;
	  struct hostent *hostInfo;

	  

	  hostInfo = gethostbyname(buf);
	  cout << buf << endl;
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
	  
	 if (quit)
		prime = -1;
	  
		prime = htonl( prime ) ;
	  
		
		if (send(socketDescriptor, &prime, sizeof(prime), 0) < 0) {
			  cerr << "cannot send data ";
			  close(socketDescriptor);
			  exit(1);
		}
		if (quit){
			close(socketDescriptor);
			cout << "exit()" <<endl;
			exit(0);
		}
		for(int k=0; k < size; k++){
			uint32_t thelong;

			thelong = htonl( list[k] ) ;
			if (send(socketDescriptor, &thelong, sizeof(thelong), 0) < 0) {
			  cerr << "cannot send data ";
			  close(socketDescriptor);
			  exit(1);
			} 
		}	

	  close(socketDescriptor);

	}

}












