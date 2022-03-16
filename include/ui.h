/*
 * @Author: Celery
 * @Date: 2022-03-16 11:01:18
 * @LastEditTime: 2022-03-16 15:18:08
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\include\ui.h
 * 
 */
#ifndef UI_H
#define UI_H

#include "common.h"
#include "display_manager.h"
#include "input_manager.h"
#include "font_manager.h"

#define BUTTON_DEFAULT_COLOR    0xff0000
#define BUTTON_PRESSED_COLOR    0x00ff00
#define BUTTON_PRECENT_COLOR    0x0000ff
#define BUTTON_TEXT_COLOR       0x000000

#define BUTTON_STATE_DEFAULT    0
#define BUTTON_STATE_PRESSED    1
#define BUTTON_STATE_PRECENT    2

#define BUTTON_TEXT_DEFAULT_SIZE 12

struct _button_obj_t;

typedef int (*DRAW_BUTTON_FUNC)(struct _button_obj_t *button, display_buff_t *display_buff);
typedef int (*PRESS_BUTTON_FUNC)(struct _button_obj_t *button, display_buff_t *display_buff, input_event_t *input_event);

typedef struct _button_obj_t {
    char *name;
    int font_size;
    int state;
    region_t region;
    DRAW_BUTTON_FUNC draw_button;
    PRESS_BUTTON_FUNC press_button;
} button_obj_t;


void button_init(char *name, button_obj_t *button, region_t region, DRAW_BUTTON_FUNC draw_button, PRESS_BUTTON_FUNC press_button);

#endif