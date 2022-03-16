/*
 * @Author: Celery
 * @Date: 2022-03-16 19:17:07
 * @LastEditTime: 2022-03-16 19:45:15
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\include\page_manager.h
 * 
 */
#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

typedef struct _page_action_t {
    char *name;
    void (*run)(void *parameter);
    struct _page_action_t *next;
} page_action_t;

page_action_t *select_page(char *name);
void register_page(page_action_t *page);
void sys_pages_register(void);

#endif