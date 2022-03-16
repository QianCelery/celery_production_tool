/*
 * @Author: Celery
 * @Date: 2022-03-16 19:41:35
 * @LastEditTime: 2022-03-16 19:46:27
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\unit_test\page_test.c
 * 
 */
#include "page_manager.h"
#include "common.h"

int main(int argc, char **argv)
{
    sys_pages_register();

    select_page("main")->run(NULL);


    return 0;
}
