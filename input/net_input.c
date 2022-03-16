/*
 * @Author: Celery
 * @Date: 2022-03-15 15:05:27
 * @LastEditTime: 2022-03-15 20:30:50
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\input\net_input.c
 * 
 */
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <string.h>

#include "input_manager.h"

#define SERVER_PORT 8888

int socket_server;

static int net_input_init(void)
{
    struct sockaddr_in socket_server_addr;
    int ret;

    socket_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_server == -1) {
        printf("socket err\n");
        return -1;
    }

    socket_server_addr.sin_family = AF_INET;
    socket_server_addr.sin_port = htons(SERVER_PORT);
    socket_server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&socket_server_addr.sin_zero, 0, 8);

    ret =  bind(socket_server, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));
    if (ret == -1) {
        printf("bind err\n");
        return -1;
    }

    return 0;
}

static int net_input_exit(void)
{
    close(socket_server);
    return 0;
}

static int net_input_get_input_event(input_event_t *input_event)
{
    struct sockaddr_in socket_client_addr;
    
    char recv_buf[1024];
    unsigned int addr_len;
    int recv_len;

    addr_len = sizeof(struct sockaddr);
    recv_len = recvfrom(socket_server, recv_buf, 999, 0, (struct sockaddr *)&socket_client_addr, &addr_len);
    if(recv_len > 0) {
        recv_buf[recv_len] = '\0';
        strcpy(input_event->str, recv_buf);
        input_event->type = INPUT_TYPE_NET;
        gettimeofday(&input_event->time, NULL);
        return 0;
    } else {
        return -1;
    }
}

input_device_t net_input_dev = {
    .name = "touchscreen",
    .input_device_init = net_input_init,
    .input_device_exit = net_input_exit,
    .get_input_event = net_input_get_input_event,
};

/**
 * @description: 注册网络输入设备
 * @param {*}
 * @return {*}
 */
void register_net_input(void)
{
    register_input_device(&net_input_dev);
}


#if 0   //临时测试内容
int main(int argc, char **argv)
{
    input_event_t tmp_input_event;
    int ret;

    net_input_dev.input_device_init();

    while (1) {
        ret = net_input_dev.get_input_event(&tmp_input_event);
        if (ret != 0) {
            printf("get_input_event err.\n");
        } else {
            printf("type: %d\n", tmp_input_event.type);
            printf("str: %s\n", tmp_input_event.str);
            printf("sec: %ld\n", tmp_input_event.time.tv_sec);
            printf("usec: %ld\n", tmp_input_event.time.tv_usec);
        }
    }

}

#endif