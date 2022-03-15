/*
 * @Author: Celery
 * @Date: 2022-03-15 14:28:54
 * @LastEditTime: 2022-03-15 18:44:51
 * @LastEditors: Celery
 * @Description: 触摸屏各项操作实心，使用tslib库
 * @FilePath: \celery_production_tool\input\touchscreen.c
 * 
 */
#include <tslib.h>
#include <stdio.h>

#include "input_manager.h"

struct tsdev *ts;

/**
 * @description: 触摸屏初始化
 * @param {*}
 * @return {*}
 */
int touchscreen_init(void)
{
    ts = ts_setup(NULL, 0);
	if (!ts)
	{
		printf("ts_setup err\n");
		return -1;
	}

    return 0;
}

/**
 * @description: 触摸屏关闭
 * @param {*}
 * @return {*}
 */
int touchscreen_exit(void)
{
    ts_close(ts);

    return 0;
}

/**
 * @description: 获取触摸屏输入
 * @param {input_event_t} *input_event
 * @return {*}
 */
int touchscreen_get_input_event(input_event_t *input_event)
{
    struct ts_sample samp;
    int ret;

    ret = ts_read(ts, &samp, 1);
	
	if (ret < 1)
		return -1;

    input_event->time = samp.tv;
    input_event->type = INPUT_TYPE_TOUCHSCREEN;
    input_event->x    = samp.x;
    input_event->y    = samp.y;
    input_event->pressure = samp.pressure;

    return 0;
}

input_device_t touchscreen_dev = {
    .name = "touchscreen",
    .input_device_init = touchscreen_init,
    .input_device_exit = touchscreen_exit,
    .get_input_event = touchscreen_get_input_event,
};

/**
 * @description: 注册触摸屏设备
 * @param {*}
 * @return {*}
 */
void register_touchscreen(void)
{
    register_input_device(&touchscreen_dev);
}

#if 0   //临时测试内容
int main(int argc, char **argv)
{
    input_event_t tmp_input_event;

    touchscreen_dev.input_device_init();

    while (1) {
        touchscreen_dev.get_input_event(&tmp_input_event);

        printf("type: %d\n", tmp_input_event.type);
        printf("x: %d\n", tmp_input_event.x);
        printf("y: %d\n", tmp_input_event.y);
        printf("pressure: %d\n", tmp_input_event.pressure);
        printf("sec: %ld\n", tmp_input_event.time.tv_sec);
        printf("usec: %ld\n", tmp_input_event.time.tv_usec);

    }
    touchscreen_dev.input_device_exit();
    return 0;
}

#endif