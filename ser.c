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

#define RANGE 1000 // rand range = 0 ~ 1000 
int cur_rand; // rand number
int lower_bound = 0;
int upper_bound = RANGE;
int serverSocket, clientSocket;
int rand_gen();
void update_range(int a);

int main(int argc, char *argv[])
{
	struct sockaddr_in serverAddress, clientAddress;
	int server_addr_length = sizeof(serverAddress);
	int client_addr_length = sizeof(clientAddress);
	int serverSocket, clientSocket;
	int ServerPortNumber;

	if(argc == 2){
		ServerPortNumber = atoi(argv[1]);
	}

   	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // create connection
	if(serverSocket < 0){
		fprintf(stderr, "Error creating socket : %s\n", strerror(errno));
		exit(0);
	}
	// buuild address data structure
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
    char *menu = "\
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
    cur_rand = rand_gen(); printf("cur_rand: %d\n", cur_rand);
	send_buf[0] = '\0';
	strcat(send_buf, menu);
    strcat(send_buf, guess);
	bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
	if(bytesSend < 0) printf("Error sending packet\n");

  	while(1){
        // get client number
		bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
		if(bytesRecv < 0) printf(" Error receiving packet\n");


        printf("%s\n", recv_buf);
        if((recv_buf[0] == 27) || !strncmp(recv_buf, "esc", 1) ) {
			break;
		}
        send_buf[0] = '\0';
        // deal with game
        /*
            1. if guess right
                new rand
                send ac+guess
        */
        int client_guess = atoi(recv_buf);
        if(client_guess == cur_rand) {
            cur_rand = rand_gen(); printf("cur_rand: %d\n", cur_rand);
            strcat(send_buf, ac);
            strcat(send_buf, guess);
        }
        /*
            2. else
                update range
                send rand+guess
        */
        else{
            update_range(client_guess);
            char tmp[50];
            strcat(send_buf, wrong);
            strcat(send_buf, high);
                sprintf(tmp, "%d", lower_bound); 
            strcat(send_buf, tmp);

            strcat(send_buf, low);
                sprintf(tmp, "%d", upper_bound); 
            strcat(send_buf, tmp);

            strcat(send_buf, guess);
        }
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