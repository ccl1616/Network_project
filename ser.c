#include <stdio.h>
#include <stdlib.h>
#include <string.h>	// for bzero()
#include <unistd.h>	// for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define RANGE 1000 // rand range = 0 ~ 999 
int cur_rand; // current rand number
int lower_bound = 0;
int upper_bound = RANGE;
int rand_gen(); // generate a random number
void update_range(int a); // update bound by client input

int serverSocket, clientSocket;

int main(int argc, char *argv[])
{
	// data structure to save info
	struct sockaddr_in serverAddress, clientAddress;
	int server_addr_length = sizeof(serverAddress);
	int client_addr_length = sizeof(clientAddress);
	int serverSocket, clientSocket;
	int ServerPortNumber;

	// check if command include port number
	if(argc == 2){
		ServerPortNumber = atoi(argv[1]);
	}
	// setup passive open
   	serverSocket = socket(PF_INET, SOCK_STREAM, 0); 
	if(serverSocket < 0){
		fprintf(stderr, "Error creating socket : %s\n", strerror(errno));
		exit(0);
	}
	// build address data structure
 	memset(&serverAddress, 0, sizeof(serverAddress)); // bzero
	serverAddress.sin_family = AF_INET;
  	serverAddress.sin_port = htons(ServerPortNumber);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	// bind socket to address
	if(bind(serverSocket,(struct sockaddr *) &serverAddress, server_addr_length) == -1){
		fprintf(stderr, "Error binding : %s\n", strerror(errno));
		close(serverSocket);
		exit(0);
	}
	// listen for connection
	if(listen(serverSocket, 3) == -1){
		fprintf(stderr, "Error listening : %s\n", strerror(errno));
		close(serverSocket);
		exit(0);
	}

	printf("Waiting for client...\n");
	if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &client_addr_length)) == -1){
		printf("accept failed\n");
		close(serverSocket);
		exit(0);
	}
	printf("Client connect successfully\n");

	int bytesRecv, bytesSend;
    char send_buf[500];
	char recv_buf[500];
    char *start = "\
\n--------------------------------\n\
Game Start\
\n--------------------------------\n";

	char *guess = "\nGuess a number:\n";

    char *wrong = "wrong\n";
	char *low = "\nlower than: ";
    char *high = "higher than: ";

	char *ac = "Answer Correct ^0^\n\
--------------------------------\n\
Start Next Round\
\n--------------------------------\n";
	
	// Welcome client
    cur_rand = rand_gen(); 
	printf("cur_rand: %d\n", cur_rand);
	send_buf[0] = '\0';
	strcat(send_buf, start);
    strcat(send_buf, guess);
	bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
	if(bytesSend < 0) printf("Error sending packet\n");

  	while(1){
        // get client input
		bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
		if(bytesRecv < 0) printf(" Error receiving packet\n");
		// check if client want to quit
        if((recv_buf[0] == 27) || !strncmp(recv_buf, "esc", 1) ) {
			break;
		}
        // deal with game
        int client_guess = atoi(recv_buf);
		printf("%d\n", client_guess);
		send_buf[0] = '\0';
        if(client_guess == cur_rand) {
			/*
				1. if guess right
					new rand
					make msg
			*/
            cur_rand = rand_gen(); 
			printf("cur_rand: %d\n", cur_rand); // show next round answer on server side
            strcat(send_buf, ac);
            strcat(send_buf, guess);
        }
        else{
			/*
				2. else
					update range
					make msg
        	*/
			// update range by input number 
            update_range(client_guess);
			// send back wrong + range + guess
            char tmp[50];
            strcat(send_buf, wrong);
            strcat(send_buf, high);
                sprintf(tmp, "%d", lower_bound); // transfer number to string
            strcat(send_buf, tmp);

            strcat(send_buf, low);
                sprintf(tmp, "%d", upper_bound); 
            strcat(send_buf, tmp);

            strcat(send_buf, guess);
        }
		// send msg to client
        bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
        if(bytesSend < 0) printf("Error sending packet\n");
	}
	close(clientSocket);
	return 0;
}

int rand_gen()
{
    time_t t;
    srand((unsigned) time(&t));
    return rand() % RANGE;
}

void update_range(int a)
{
    if(a < cur_rand && a > lower_bound) lower_bound = a;
    if(a > cur_rand && a < upper_bound) upper_bound = a;
}