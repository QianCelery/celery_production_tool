/*
 * @Author: Celery
 * @Date: 2022-03-13 21:14:57
 * @LastEditTime: 2022-03-16 14:42:50
 * @LastEditors: Celery
 * @Description: 显示设备管理器头文件
 *               各项数据类型定义、提供外部接口
 * @FilePath: \celery_production_tool\include\display_manager.h
 * 
 */
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "common.h"
#include "font_manager.h"

/**
 * @description: 显示缓存结构体
 * 提供x y分辨率 bits_per_pixel 缓存基地址 
 */
typedef struct {
    int x_res;
    int y_res;
    int bpp;
    unsigned char *buff;
} display_buff_t;

/**
 * @description: 显示设备操作结构体
 * 各显示设备单独实现，再注册进display_manager中的设备链表中 
 */
typedef struct _display_operations_t {
	char *name;
    int (*dev_init)(void);
    void (*dev_exit)(void);
    int (*get_buffer)(display_buff_t *display_buf);
    int (*flush_region)(region_t *region, display_buff_t *display_buf);
    struct _display_operations_t *next;
} display_operations_t;

void draw_text_in_region_central(char *text, region_t region_t, unsigned int color);
void draw_font_bitmap(font_bitmap_t font_bitmap, unsigned int color);
void draw_region(region_t *region, unsigned int color);
void put_pixel(int x, int y, unsigned int color);
int set_default_display(char *name);
int default_display_init(void);
display_buff_t *get_display_buff(void);
void flush_display_buff(region_t *region, display_buff_t *display_buff);
void display_register(display_operations_t *display_opr);
void sys_display_register(void);

#endif