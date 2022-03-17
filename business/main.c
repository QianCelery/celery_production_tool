/*
 * @Author: Celery
 * @Date: 2022-03-17 14:53:08
 * @LastEditTime: 2022-03-17 21:24:51
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\business\main.c
 * 
 */
#include <stdio.h>

#include "common.h"
#include "page_manager.h"
#include "display_manager.h"
#include "font_manager.h"
#include "input_manager.h"

int main(int argc, char **argv)
{
    int ret;

    if (argc != 2) {
        printf("Usage: %s <font_file>\n", argv[0]);
        return -1;
    }

    /*初始化显示设备管理器*/
    sys_display_register();
    set_default_display("fb");
    default_display_init();

    /*初始化输入设备管理器*/
    sys_input_device_register();
    input_device_init();

    /*初始化字体管理器*/
    sys_font_opr_register();
    ret = select_init_font("freetype", argv[1]);
	if (ret) {
		printf("select_init_font err.\n");
	}

    /*初始化页面管理器*/
    sys_pages_register();

    /*业务主界面*/
    select_page("main")->run(NULL);

    return 0;
}