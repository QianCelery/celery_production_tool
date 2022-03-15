/*
 * @Author: Celery
 * @Date: 2022-03-14 21:04:44
 * @LastEditTime: 2022-03-15 18:48:53
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\include\input_manager.h
 * 
 */
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCHSCREEN  1    
#define INPUT_TYPE_NET          2

typedef struct {
    struct timeval time;
    int type;
    //触摸屏输入数据域
    int x;
    int y;
    int pressure;
    //网络输入数据域
    char str[1024]; 
} input_event_t;

typedef struct _input_device_t {
    char *name;
    int (*get_input_event)(input_event_t *input_event);
    int (*input_device_init)(void);
    int (*input_device_exit)(void);
    struct _input_device_t *next;
} input_device_t;

void register_input_device(input_device_t *input_dev);
void sys_input_device_register(void);
int input_device_init(void);
int get_input_event(input_event_t *input_event);

#endif