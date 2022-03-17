/*
 * @Author: Celery
 * @Date: 2022-03-17 14:53:34
 * @LastEditTime: 2022-03-17 15:41:30
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\config\config.c
 * 
 */
#include <stdio.h>
#include <string.h>

#include "config.h"

static item_cfg_t g_item_cfg[ITEM_CFG_MAX_NUM];
static int g_item_cfg_count;

/**
 * @description: 处理config_file文件  CONFIG_FILE在config中定义
 *              提取其中的name can_be_touched command
 * @param {*}
 * @return {0:处理成功 -1:处理失败}
 */
int prase_config_file(void)
{
    FILE *filp;
    char buff[100];
    char *p;

    filp = fopen(CONFIG_FILE, "r");
    if (!filp) {
        printf("prase_config_file -> fopen err.\n");
        return -1;
    }

    while (fgets(buff, 100, filp)) {
        buff[99] = '\0';
        p = buff;
        /* 去除空格与tab */
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        /* 忽略注释 */
        if (*p == '#') {
            continue;
        }

        /* 处理配置信息 */
        g_item_cfg[g_item_cfg_count].command[0] = '\0'; //有可能该项目未配置对应命令
        sscanf(p, "%s %d %s",   g_item_cfg[g_item_cfg_count].name, \
                                &g_item_cfg[g_item_cfg_count].can_be_touched, \
                                g_item_cfg[g_item_cfg_count].command);
        g_item_cfg_count++;
    }

    fclose(filp);

    return 0;

}

int get_item_count(void)
{
    return g_item_cfg_count;
}

item_cfg_t *get_item_cfg_by_name(char *name)
{
    int i = 0;

    for (i = 0; i != g_item_cfg_count; ++i) {
        if (strcmp(name, g_item_cfg[i].name) == 0) {
            return &g_item_cfg[i];
        }
    }

    return NULL;
}

item_cfg_t *get_item_cfg_by_index(int index)
{
    if (index < g_item_cfg_count) {
        return &g_item_cfg[index];
    } else {
        return NULL;
    }
}