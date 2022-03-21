/*
 * @Author: Celery
 * @Date: 2022-03-15 19:47:09
 * @LastEditTime: 2022-03-21 20:49:03
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\include\font_manager.h
 * 
 */
#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "common.h"

/**
 * @brief 字体bit_map数据结构实现
 * region:LCD显示区域
 * origin:矢量字体基点
 * buff:点阵map
 */
typedef struct _font_bitmap_t {
    region_t region;
    int cur_origin_x;
    int cur_origin_y;
    int next_origin_x;
    int next_origin_y;
    unsigned char *buff;
} font_bitmap_t;

/**
 * @brief 字体操作数据结构定义
 * 
 */
typedef struct _font_operation_t {
    char *name;
    int (*font_init)(char *file_name);
    int (*set_font_size)(int font_size);
    int (*get_font_bitmap)(unsigned int code, font_bitmap_t *font_bitmap);
    int (*get_string_region_cartesian)(char *str, region_cartesian_t *region_car);
    struct _font_operation_t *next;
} font_operation_t;

void register_font_opr(font_operation_t *font_opr);
void sys_font_opr_register(void);
int select_init_font(char *font_opr_name, char *filename);
int set_font_size(int font_size);
int get_font_bitmap(unsigned int code, font_bitmap_t *font_bitmap);
int get_string_region_cartesian(char *str, region_cartesian_t *region_car);


#endif