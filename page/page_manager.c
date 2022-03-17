/*
 * @Author: Celery
 * @Date: 2022-03-16 19:17:20
 * @LastEditTime: 2022-03-16 21:02:24
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\page\page_manager.c
 * 
 */
#include <stdio.h>
#include <string.h>

#include "page_manager.h"

page_action_t *sys_pages = NULL;

page_action_t *select_page(char *name)
{
    page_action_t *tmp_page = sys_pages;

    while (tmp_page) {
        if (strcmp(name, tmp_page->name) == 0) {
            return tmp_page;
        }
        tmp_page = tmp_page->next;
    }
                             

    return NULL;
}

void register_page(page_action_t *page)
{
    page->next = sys_pages;
    sys_pages = page;
}

void sys_pages_register(void)
{
    extern void register_main_page(void);
    register_main_page();
}


