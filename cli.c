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

#define SERVER_PORT 5432
#define MAX_LINE 256

int main(int argc, char *argv[] )
{
    FILE *fp;
    struct hostent *hp;
}