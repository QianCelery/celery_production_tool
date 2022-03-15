/*
 * @Author: Celery
 * @Date: 2022-03-15 17:19:53
 * @LastEditTime: 2022-03-15 18:50:33
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\unit_test\input_test.c
 * 
 */
#include "input_manager.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    input_event_t event;

    sys_input_device_register();

    input_device_init();

    while (1) {
        if (get_input_event(&event) == 0) {
            printf("get input event:\n");
            if (event.type == INPUT_TYPE_TOUCHSCREEN) {
                printf("type: %d\n", event.type);
                printf("x: %d\n", event.x);
                printf("y: %d\n", event.y);
                printf("pressure: %d\n", event.pressure);
                printf("sec: %ld\n", event.time.tv_sec);
                printf("usec: %ld\n", event.time.tv_usec);
            } else if(event.type == INPUT_TYPE_NET) {
                printf("type: %d\n", event.type);
                printf("str: %s\n", event.str);
                printf("sec: %ld\n", event.time.tv_sec);
                printf("usec: %ld\n", event.time.tv_usec);
            }
        } else {
            printf("get input event err\n");
        }
    }
    
    return 0;
}