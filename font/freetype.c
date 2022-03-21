/*
 * @Author: Celery
 * @Date: 2022-03-15 19:46:17
 * @LastEditTime: 2022-03-21 20:45:20
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

/**
 * @description: 获取输入字符串的边框
 * @param {char} *str
 * @param {region_cartesian_t} *region_car
 * @return {*}
 */
static int freetype_get_string_region_carsesian(char *str, region_cartesian_t *region_car)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_face->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < strlen(str); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(g_face, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(g_face, str[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(g_face->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    region_car->x_res = bbox.xMin;
    region_car->y_res = bbox.yMax;
    region_car->width = bbox.xMax - bbox.xMin + 1;
    region_car->hight = bbox.yMax - bbox.yMin + 1;

	return 0;	
}

font_operation_t freetype_opr = {
    .name = "freetype",
    .font_init = freetype_font_init,
    .set_font_size = freetype_set_font_size,
    .get_font_bitmap = freetype_get_font_bitmap,
    .get_string_region_cartesian = freetype_get_string_region_carsesian,
};

void register_freetype_opr(void)
{
    register_font_opr(&freetype_opr);
}