/*
 * @Author: Celery
 * @Date: 2022-03-16 19:17:40
 * @LastEditTime: 2022-03-16 19:39:30
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\page\main_page.c
 * 
 */
#include "page_manager.h"
#include <stdio.h>

void main_page_run(void *parameter)
{
    printf("main page running.\n");
}

page_action_t main_page = {
    .name = "main",
    .run = main_page_run,
};

void register_main_page(void)
{
    register_page(&main_page);
}