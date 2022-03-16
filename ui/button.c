/*
 * @Author: Celery
 * @Date: 2022-03-16 11:01:04
 * @LastEditTime: 2022-03-16 15:20:02
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\ui\button.c
 * 
 */
#include <stdio.h>

#include "ui.h"

/**
 * @description:  默认的绘制按键函数
 * @param {button_obj_t} *button
 * @param {display_buff_t} *display_buff
 * @return {*}
 */
static int default_draw_button(button_obj_t *button, display_buff_t *display_buff)
{
    //绘制方框底色
    draw_region(&button->region, BUTTON_DEFAULT_COLOR);

    //居中显示按键内容
    set_font_size(button->font_size);
    draw_text_in_region_central(button->name, button->region, BUTTON_TEXT_COLOR);

    //flush buffer
    flush_display_buff(&button->region, display_buff);

    return 0;
}

/**
 * @description: 默认的按键按下ui处理函数
 * @param {button_obj_t} *button
 * @param {display_buff_t} *display_buff
 * @param {input_event_t} input_event
 * @return {*}
 */
static int default_press_button(button_obj_t *button, display_buff_t *display_buff, input_event_t *input_event)
{
    unsigned int color = BUTTON_DEFAULT_COLOR;

    if (button->state) {
        color = BUTTON_PRESSED_COLOR;
    }

    //绘制方框底色
    draw_region(&button->region, color);

    //居中显示按键内容
    set_font_size(button->font_size);
    draw_text_in_region_central(button->name, button->region, BUTTON_TEXT_COLOR);

    //flush buffer
    flush_display_buff(&button->region, display_buff);

    return 0;
}

/**
 * @description: 初始化按键
 * @param {char} *name
 * @param {button_obj_t} *button
 * @param {region_t} region
 * @param {DRAW_BUTTON_FUNC} draw_button
 * @param {PRESS_BUTTON_FUNC} press_button
 * @return {*}
 */
void button_init(char *name, button_obj_t *button, region_t region, DRAW_BUTTON_FUNC draw_button, PRESS_BUTTON_FUNC press_button)
{
    button->name = name;
    button->state = BUTTON_STATE_DEFAULT; 
    button->font_size = BUTTON_TEXT_DEFAULT_SIZE;
    button->region = region;
    button->draw_button = draw_button ? draw_button : default_draw_button;
    button->press_button = press_button ? press_button : default_press_button;
}