/*
 * @Author: Celery
 * @Date: 2022-03-13 21:12:18
 * @LastEditTime: 2022-03-21 21:23:43
 * @LastEditors: Celery
 * @Description: 显示设备管理器，实现对各类显示设备的注册与操作
 * @FilePath: \celery_production_tool\display\display_manager.c
 * 
 */
#include <stdio.h>
#include <string.h>

#include "display_manager.h"
#include "font_manager.h"

//管理底层的显示设备，LCD、WEB显示等
static display_operations_t *sys_display_opr = NULL;
static display_operations_t *default_display_opr = NULL;
static display_buff_t cur_display_buff;
static unsigned int line_width;
static unsigned int pixel_width;

/**
 * @description: 在指定区域绘制文本信息
 * @param {char} *text
 * @param {region_t} region_t
 * @param {unsigned int} color
 * @return {*}
 */
void draw_text_in_region_central(char *text, region_t region, unsigned int color)
{
    int i = 0;
    int ret;
    font_bitmap_t font_bitmap;
    region_cartesian_t region_car;

    int origin_x;
    int origin_y;

    get_string_region_cartesian(text, &region_car);

    origin_x = region.x_res + (region.width - region_car.width) / 2 + region_car.x_res;
    origin_y = region.y_res + (region.hight - region_car.hight) / 2 + region_car.y_res;

    for (i = 0; text[i] != '\0'; ++i) {
        font_bitmap.cur_origin_x = origin_x;
        font_bitmap.cur_origin_y = origin_y;
        ret = get_font_bitmap(text[i], &font_bitmap);
        if (ret) {
            printf("draw_text_in_region_central -> get_font_bitmap err.\n");
            return;
        }
        draw_font_bitmap(font_bitmap, color);
        origin_x = font_bitmap.next_origin_x;
        origin_y = font_bitmap.next_origin_y;
    }

}

/**
 * @description: 绘制字体bitmap
 * @param {font_bitmap_t} font_bitmap
 * @param {unsigned int} color
 * @return {*}
 */
void draw_font_bitmap(font_bitmap_t font_bitmap, unsigned int color)
{
    int x = font_bitmap.region.x_res;
    int y = font_bitmap.region.y_res;
    int width = font_bitmap.region.width;
    int hight = font_bitmap.region.hight;
    int i, j;

    for (i = 0; i != hight; ++i) {
        for (j = 0; j != width; ++j) {
            //越界超出显示范围不显示
            if ((j + x) > cur_display_buff.x_res || (i + y) > cur_display_buff.y_res) 
                continue;
            if (font_bitmap.buff[i * width + j]) {
                put_pixel(j + x, i + y, color);
            }
        }
    }
}

/**
 * @description: 绘制region
 * @param {region_t} *region
 * @param {unsigned int} color
 * @return {*}
 */
void draw_region(region_t *region, unsigned int color)
{
    int x = region->x_res;
    int y = region->y_res;
    int width = region->width;
    int hight = region->hight;

    int i, j;

    for (i = 0; i != hight; ++i) {
        for (j = 0; j != width; ++j) {
            put_pixel(x + j, y + i, color);
        }
    }
}

/**
 * @description: 描点，在LCD指定位置输出指定颜色
 * @param {int} x
 * @param {int} y
 * @param {unsigned int} color
 * @return {*}
 */
void put_pixel(int x, int y, unsigned int color)
{
    unsigned char *bpp_8 = (unsigned char *)cur_display_buff.buff + x * pixel_width + y * line_width;
    unsigned short *bpp_16;
    unsigned int *bpp_32;

    unsigned int red, green, blue;

    bpp_16 = (unsigned short *)bpp_8;
    bpp_32 = (unsigned int *)bpp_8;

    switch (cur_display_buff.bpp) {
        case 8: 
            *bpp_8 = color;
            break;
        case 16: //565
            red = (color >> 16) & 0xff;
            green = (color >> 8) & 0xff;
            blue = color & 0xff;
            *bpp_16 = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            break;
        case 32: 
            *bpp_32 = color;
            break;
        default : 
            printf("Can't support %dbpp.\n", cur_display_buff.bpp);
            break;
    }
}

/**
 * @description: 使用name字段设置默认显示设备
 * @param {char} *name
 * @return {*}
 */
int set_default_display(char *name)
{
    display_operations_t *tmp = sys_display_opr;
    while (tmp) {
        if (strcmp(name, tmp->name) == 0) {
            default_display_opr = tmp;
            return 0;
        }
        tmp = tmp->next;
        
    }

    return -1;
}

/**
 * @description: 默认显示设备初始化
 * @param {none}
 * @return {*}
 */
int default_display_init(void)
{
    int ret;

    ret = default_display_opr->dev_init();
    if (ret != 0) {
        printf("default_display_opr->dev_init() err.\n");
        return -1;
    }

    ret = default_display_opr->get_buffer(&cur_display_buff);
    if (ret != 0) {
        printf("default_display_opr->get_buffer(cur_display_buff) err.\n");
        return -1;
    }

    line_width = cur_display_buff.x_res * cur_display_buff.bpp / 8;
    pixel_width = cur_display_buff.bpp / 8;

    return 0;
}

/**
 * @description: 获取当前显示缓存
 * @param {*}
 * @return {*}
 */
display_buff_t *get_display_buff(void)
{
    return &cur_display_buff;
}

/**
 * @description: 将region内容刷新至显示缓存
 * @param {*}
 * @return {*}
 */
void flush_display_buff(region_t *region, display_buff_t *display_buff)
{
    default_display_opr->flush_region(region, display_buff);
}

/**
 * @description: 显示设备注册，插入sys_display_opr链表头
 * @param {display_operations_t} *display_opr
 * @return {*}
 */
void display_register(display_operations_t *display_opr)
{
    display_opr->next = sys_display_opr;
    sys_display_opr = display_opr;
}

/**
 * @description: 系统内显示设备注册
 * @param {*}
 * @return {*}
 */
void sys_display_register(void)
{
    extern void framebuffer_register(void);
    framebuffer_register();
}