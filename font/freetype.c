/*
 * @Author: Celery
 * @Date: 2022-03-15 19:46:17
 * @LastEditTime: 2022-03-16 10:03:32
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\font\freetype.c
 * 
 */
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "font_manager.h"

static FT_Face g_face;
static int g_default_font_size = 24;

/**
 * @description: freetype初始化
 * @param {char} *file_name
 * @return {*}
 */
static int freetype_font_init(char *file_name)
{
    FT_Library	  library;
    int error;
	
    /* 显示矢量字体 */
	error = FT_Init_FreeType( &library );			   /* initialize library */
	if (error) {
		printf("FT_Init_FreeType error\n");
		return -1;
	}
	
	error = FT_New_Face( library, file_name, 0, &g_face ); /* create g_face object */
	if (error) {
		printf("FT_New_Face error\n");
		return -1;
	}	

	FT_Set_Pixel_Sizes(g_face, g_default_font_size, 0);

    return 0;
}

/**
 * @description: 设置字体
 * @param {int} font_size
 * @return {*}
 */
static int freetype_set_font_size(int font_size)
{
    FT_Set_Pixel_Sizes(g_face, font_size, 0);
    return 0;
}

/**
 * @description: 获取bitmap
 * @param {unsigned int} code 需要获取的字符
 * @param {font_bitmap_t} *font_bitmap
 *          需要传入所绘制的基点，由此计算bitmap和下个map的基点
 * @return {*}
 */
static int freetype_get_font_bitmap(unsigned int code, font_bitmap_t *font_bitmap)
{
    int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_face->glyph;

    pen.x = font_bitmap->cur_origin_x *64;  //单位:1/64像素
    pen.y = font_bitmap->cur_origin_y *64;  //单位:1/64像素

    /* 转换：transformation */
	FT_Set_Transform(g_face, 0, &pen);

    /* 加载位图: load glyph image into the slot (erase previous one) */
	error = FT_Load_Char(g_face, code, FT_LOAD_RENDER);
	if (error) {
		printf("FT_Load_Char error\n");
		return -1;
	}

    font_bitmap->buff = slot->bitmap.buffer;

    font_bitmap->region.x_res = slot->bitmap_left;
    font_bitmap->region.y_res = font_bitmap->cur_origin_y * 2 - slot->bitmap_top;
    font_bitmap->region.width = slot->bitmap.width;
    font_bitmap->region.hight = slot->bitmap.rows;
    font_bitmap->next_origin_x = font_bitmap->cur_origin_x + slot->advance.x / 64;
    font_bitmap->next_origin_y = font_bitmap->cur_origin_y;

    return 0;
}

font_operation_t freetype_opr = {
    .name = "freetype",
    .font_init = freetype_font_init,
    .set_font_size = freetype_set_font_size,
    .get_font_bitmap = freetype_get_font_bitmap,
};

void register_freetype_opr(void)
{
    register_font_opr(&freetype_opr);
}