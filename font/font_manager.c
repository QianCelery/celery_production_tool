/*
 * @Author: Celery
 * @Date: 2022-03-15 19:46:27
 * @LastEditTime: 2022-03-16 09:52:00
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\font\font_manager.c
 * 
 */
#include <string.h>

#include "font_manager.h"

font_operation_t *sys_font_opr = NULL;
font_operation_t *default_font_opr = NULL;

/**
 * @description: 注册font_opr至sys_font_opr链表
 * @param {font_operation_t} *font_opr
 * @return {*}
 */
void register_font_opr(font_operation_t *font_opr)
{
    font_opr->next = sys_font_opr;
    sys_font_opr = font_opr;
}

/**
 * @description: 注册系统内所包含的font_opr
 * @param {*}
 * @return {*}
 */
void sys_font_opr_register(void)
{
    extern void register_freetype_opr(void);
    register_freetype_opr();
}

/**
 * @description: 
 * @param {char} *font_opr_name
 * @param {char} *filename
 * @return {*}
 */
int select_init_font(char *font_opr_name, char *filename)
{
    font_operation_t *tmp_font_opr = sys_font_opr;

    while (tmp_font_opr) {
        if (strcmp(font_opr_name, tmp_font_opr->name) == 0) {
            default_font_opr = tmp_font_opr;
            break;
        }
        tmp_font_opr = tmp_font_opr->next;
    }

    if (!tmp_font_opr) {
        return -1;
    }

    return default_font_opr->font_init(filename);
}

/**
 * @description: 设置默认显示系统字体大小
 * @param {int} font_size
 * @return {*}
 */
int set_font_size(int font_size)
{
    return default_font_opr->set_font_size(font_size);
}

/**
 * @description: 从默认显示系统获取bitmap
 * @param {unsigned int} code
 * @param {font_bitmap_t} *font_bitmap
 * @return {*}
 */
int get_font_bitmap(unsigned int code, font_bitmap_t *font_bitmap)
{
    return default_font_opr->get_font_bitmap(code, font_bitmap);
}