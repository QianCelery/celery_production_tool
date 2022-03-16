/*
 * @Author: Celery
 * @Date: 2022-03-13 21:13:01
 * @LastEditTime: 2022-03-16 15:28:20
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\unit_test\ui_test.c
 * 
 */
#include <stdio.h>
#include <unistd.h>

#include "display_manager.h"
#include "font_manager.h"
#include "ui.h"


int main(int argc, char **argv)
{
	int i;
	int ret;
	char str[] = "celery production tool";
	font_bitmap_t font_bitmap;
    region_t region;
    display_buff_t *display_buff;
	button_obj_t button;

	int lcd_x = 100;
	int lcd_y = 100;

    sys_display_register();
    
    set_default_display("fb");

    default_display_init();

	sys_font_opr_register();
	ret = select_init_font("freetype", "FZSTK.TTF");
	if (ret) {
		printf("select_init_font err.\n");
	}
	
	display_buff = get_display_buff();
	printf("x_res: %d\n", display_buff->x_res);
	printf("y_res: %d\n", display_buff->y_res);
	
	for (i = 0; str[i] != '\0'; ++i) {
		font_bitmap.cur_origin_x = lcd_x;
		font_bitmap.cur_origin_y = lcd_y;
		get_font_bitmap(str[i], &font_bitmap);
		draw_font_bitmap(font_bitmap, 0xffffff);
		flush_display_buff(&font_bitmap.region, display_buff);

		lcd_x = font_bitmap.next_origin_x;
		lcd_y = font_bitmap.next_origin_y;
	}

    region.x_res = 300;
    region.y_res = 300;
    region.width = 200;
    region.hight = 80;

	// button_init("celery", &button, region, NULL, NULL);
	// button.draw_button(&button, display_buff);
	// while (1) {
	// 	button.press_button(&button, display_buff, NULL);
	// 	sleep(2);
	// }

    return 0;
}