#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/select.h>


int main(int argc, char **argv)
{
    int ifd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = fcntl(ifd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(ifd, F_SETFL, flag);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr.s_addr));
    server_addr.sin_port = htons(8888);

    connect(ifd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    perror("connect");
    fd_set set;
    FD_ZERO(&set);
    FD_SET(ifd, &set);
    struct timeval to = {
        5,0
    };
    

    int sel_ret = -1;
    if (0 !=(sel_ret =  select(ifd+1, NULL, &set,NULL, &to)))
    {
        printf("%d\n", sel_ret);

        perror("select");
        send(ifd, "hello", 6, 0);
        perror("select");
    }
    
    close(ifd);
}
