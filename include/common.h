/*
 * @Author: Celery
 * @Date: 2022-03-15 19:44:58
 * @LastEditTime: 2022-03-21 20:33:41
 * @LastEditors: Celery
 * @Description: 通用数据结构定义
 * @FilePath: \celery_production_tool\include\common.h
 * 
 */
#ifndef COMMON_H
#define COMMON_H

#ifndef NULL
#define NULL (void *)0
#endif

/**
 * @description: 显示区域结构体
 * 区域为长方体，提供左上坐标与长、宽
 */
typedef struct _region {
    int x_res;
    int y_res;
    int width;
    int hight;
} region_t;

typedef struct _region_cartesian_t {
    int x_res;
    int y_res;
    int width;
    int hight;
} region_cartesian_t;


#endif