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

#define BUF_LEN 500
#define RANGE 100
int q; // rand number
int lower_bound = 0;
int upper_bound = RANGE;
int serverSocket, clientSocket;
void close_socket(char *msg);
int rand_gen();
int str_to_int(char *str);
void start_new_round();
int check_number(int input);

int main(int argc, char *argv[])
{
	struct sockaddr_in serverAddress, clientAddress;
	int server_addr_length = sizeof(serverAddress);
	int client_addr_length = sizeof(clientAddress);
	int ServerPortNumber;
    // get port num
	if(argc == 2){
		ServerPortNumber = atoi(argv[1]);
	}
    // create connection
   	serverSocket = socket(PF_INET, SOCK_STREAM, 0); 
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

	printf("Waiting for client\n");
	if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &client_addr_length)) == -1){
		printf("Accept failed\n");
		close(serverSocket);
		exit(0);
	}
	printf("Client connect successfully\n");

	int i;
	int bytesRecv, bytesSend;
    char send_buf[BUF_LEN];
	char recv_buf[BUF_LEN];
	char str[100];
    char *menu = "Press 'y' to play or 'ESC' to quit:";

	char *new_round = "New Round:\n";

	char *ac = "Answer Correct!!! ^0^\n";

    char *tmp;

	char mes[BUF_LEN];
	mes[0] = '\0';
	// Send menu to client
    send_buf[0] = '\0';
    strcat(send_buf, menu); // cat menu to buf
    bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
    if(bytesSend < 0) close_socket("Error sending packet, menu");

    // get feedback , the y
    // check if it is ESC
    bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
	if(bytesRecv < 0) close_socket("Error receiving packet, the y");
    if(recv_buf[0] == 27) close_socket("ESC pressed"); //close if its ESC

    // start new round
    send_buf[0] = '\0';
    strcat(send_buf, new_round); // cat new round
    bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
    if(bytesSend < 0) close_socket("Error sending packet, new round");
    start_new_round();
    printf("q: %d\n", q);

  	while(1){
        // get client number
        bytesRecv = recv(clientSocket, recv_buf, sizeof(recv_buf), 0);
        if(bytesRecv < 0) close_socket("Error receiving packet, input");
        if(recv_buf[0] == 27) close_socket("ESC pressed");
        int temp = atoi(recv_buf);
        printf("user_num: %d\n", temp);

        // go check
        send_buf[0] = '\0';
        if(check_number(temp)) {
            start_new_round();
            strcat(send_buf, ac);
            strcat(send_buf, new_round);
        }
        else {
            strcat(send_buf, "higher than ");
                sprintf(tmp, "%d", lower_bound); 
            strcat(send_buf, tmp);
            strcat(send_buf, " , lower than ");
                sprintf(tmp, "%d", upper_bound); 
            strcat(send_buf, tmp);
            strcat(send_buf, ".\nkeep guessing:");
        }
        // send back result to client
        bytesSend = send(clientSocket, send_buf, sizeof(send_buf), 0);
		if(bytesSend < 0) close_socket("Error sending packet, send back result");
	}
	close(clientSocket);
	return 0;
}

void close_socket(char *msg)
{
    printf("%s\n", msg);
    close(clientSocket);
    exit(0);
}
int rand_gen()
{
    time_t t;
    srand((unsigned) time(&t));
    return rand() % RANGE;
}
int str_to_int(char *str)
{
    int val = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        val *= 10;
        val += str[i]-'0';
        i ++;
        // printf("%d\n", val);
    }
    return val;
}
void start_new_round()
{
    q = rand_gen();
    lower_bound = 0;
    upper_bound = RANGE;
    return;
}
int check_number(int a)
{
    if(a != q) {
        if(a < q && a > lower_bound) lower_bound = a;
        if(a > q && a < upper_bound) upper_bound = a;
        return 0;
    }
    else {
        return 1;
    }
}