/*
 * @Author: Celery
 * @Date: 2022-03-14 21:04:29
 * @LastEditTime: 2022-03-15 18:48:11
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\input\input_manager.c
 * 
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "input_manager.h"

static input_device_t *sys_input_dev = NULL;   //输入设备链表

static pthread_cond_t  g_convar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t g_mutex  = PTHREAD_MUTEX_INITIALIZER;

#define QUEEN_SIZE  100
#define ARRAY_SIZE  (QUEEN_SIZE + 1)    //为了方便判断队列为空情况，数组中一个元素始终不使用
#define QUEEN_TYPE input_event_t

static QUEEN_TYPE queen_input_event[ARRAY_SIZE];    //输入事件队列
static size_t front = 1;
static size_t rear = 0;

/**
 * @description: 判断队列是否为空，因为当队列只有一个元素时，front和rear都指向这个元素
 * @param {*}
 * @return {*}
 */
bool is_queen_empty(void)
{
    return (rear + 1) % ARRAY_SIZE == front;
}

/**
 * @description: 判断队列是否已满，因为数组始终有一个元素不使用，+2后等于等于front表示已满
 * @param {*}
 * @return {*}
 */
bool is_queen_full(void)
{
    return (rear + 2) % ARRAY_SIZE == front;
}

/**
 * @description: 在队列尾部插入
 * @param {QUEEN_TYPE} value
 * @return {*}
 */
void queen_insert(QUEEN_TYPE value)
{
    assert(!is_queen_full());
    rear = (rear + 1) % ARRAY_SIZE;
    queen_input_event[rear] = value;
}

/**
 * @description: 在队列头部删除
 * @param {*}
 * @return {*}
 */
void queen_delete(void)
{
    assert(!is_queen_empty());
    front = (front + 1) % ARRAY_SIZE;
}

/**
 * @description: 返回队列头部元素
 * @param {*}
 * @return {*}
 */
QUEEN_TYPE queen_first(void)
{
    assert(!is_queen_empty());
    return queen_input_event[front];
}

/**
 * @description: 获取头部元素，并在头部删除
 * @param {QUEEN_TYPE} *value
 * @return {0：获取成功 -1：获取失败}
 */
int queen_put(QUEEN_TYPE *value)
{
    if (is_queen_empty()) {
        return -1;
    } else {
        *value = queen_first();
        queen_delete();
    }

    return 0;
}

/**
 * @description: 输入设备接收线程
 * @param {void} *data 创建线程时传入(input_device_t *)
 * @return {*}
 */
static void *input_recv_thread_func(void *data)
{
    input_device_t *input_dev = (input_device_t *)data;
    input_event_t input_event;
    int ret;

    while (1) {
        ret = input_dev->get_input_event(&input_event);

        if (ret == 0) {
            pthread_mutex_lock(&g_mutex);
            queen_insert(input_event);
            pthread_cond_signal(&g_convar);    //通知接收线程
            pthread_mutex_unlock(&g_mutex);
        }
    }

    return NULL;
}

/**
 * @description: 设备注册函数，在设备实现文件中供设备注册使用
 *                  注册的设备由sys_input_dev单向链表维护
 * @param {input_device_t} *input_dev
 * @return {*}
 */
void register_input_device(input_device_t *input_dev)
{
    input_dev->next = sys_input_dev;
    sys_input_dev = input_dev;
}

/**
 * @description: 注册系统下挂的所有输入设备注册
 * @param {*}
 * @return {*}
 */
void sys_input_device_register(void)
{
    extern void register_touchscreen(void);
    register_touchscreen();

    extern void register_net_input(void);
    register_net_input();
}

/**
 * @description: 完成所有下挂输入设备初始化
 * @param {*}
 * @return {*}
 */
int input_device_init(void)
{
    int ret;
    pthread_t tid;

    input_device_t *tmp_input_dev = sys_input_dev;

    while (tmp_input_dev) {
        ret = tmp_input_dev->input_device_init();
        if (ret != 0) {
            printf("initialize device err!\n");
            return -1;
        }
        //创建该输入设备对应输入接收线程
        ret = pthread_create(&tid,  NULL, input_recv_thread_func, tmp_input_dev);
        if (ret != 0) {
            printf("Create thread err!\n");
            return -1;
        }
        tmp_input_dev = tmp_input_dev->next;
    }

    return 0;
    
}

/**
 * @description: 获取输入事件
 * @param {input_event_t} *input_event
 * @return {0:获取成功 1：获取失败}
 */
int get_input_event(input_event_t *input_event)
{
    int ret;

    pthread_mutex_lock(&g_mutex);
    if (queen_put(input_event) == 0) {
        pthread_mutex_unlock(&g_mutex);
        return 0;
    } else {
        /**
         * @brief 等待pthread cond条件满足，若满足，则获取g_mutex量，并执行后续操作
         * 若不满足，则释放互斥量
         */
        pthread_cond_wait(&g_convar, &g_mutex);
        if (queen_put(input_event) == 0) {
            ret = 0;
        } else {
            ret = -1;
        }
        pthread_mutex_unlock(&g_mutex);
    }

    return ret;
}

