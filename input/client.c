/*
 * @Author: Celery
 * @Date: 2022-02-28 21:09:04
 * @LastEditTime: 2022-03-01 15:42:46
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \APP\socket\udp\client.c
 * 
 */
/**
 * socket
 * connect/可有可无
 * send(若有connect)/sendto(若无connect)/recv/recvfrom
 * 
 */

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;
    int iAddrLen;

    int iRet;

    int iSendLen;
    unsigned char iSendBuf[1000];

    if(argc < 2)
    {
        printf("Usage : %s <server_ip>\n", argv[0]);
        return -1;
    }
    
    iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT);
    if(inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0)
    {
        printf("invalid server_ip.\n");
        return -1;
    }
    memset(&tSocketServerAddr.sin_zero, 0, 8);
    #if 0
    iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (iRet == -1)
    {
        printf("connect err.\n");
        return -1;
    }
    #endif

    iAddrLen = sizeof(struct sockaddr_in);
    while (1)
    {
        if (fgets(iSendBuf, 999, stdin))
        {
            iSendLen = sendto(iSocketClient, iSendBuf, strlen(iSendBuf), 0, (const struct sockaddr *)&tSocketServerAddr, iAddrLen);

            if(iSendLen == -1)
            {
                printf("send err.\n");
                close(iSocketClient);
                return -1;
            }
        }
    }

    close(iSocketClient);

    return 0;
}