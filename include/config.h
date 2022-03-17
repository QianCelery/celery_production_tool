/*
 * @Author: Celery
 * @Date: 2022-03-17 15:08:10
 * @LastEditTime: 2022-03-17 15:40:16
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\include\config.h
 * 
 */
#ifndef CONFIG_H
#define CONFIG_H

#define ITEM_CFG_MAX_NUM    30
#define CONFIG_FILE         "/etc/gui.conf"

typedef struct _item_cfg_t {
    int index;
    char name[64];
    int can_be_touched;
    char command[64];
} item_cfg_t;

int prase_config_file(void);
int get_item_count(void);
item_cfg_t *get_item_cfg_by_name(char *name);
item_cfg_t *get_item_cfg_by_index(int index);

#endif