/*
 * @Author: Celery
 * @Date: 2022-03-16 19:17:40
 * @LastEditTime: 2022-03-21 21:52:55
 * @LastEditors: Celery
 * @Description: 
 * @FilePath: \celery_production_tool\page\main_page.c
 * 
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#include "display_manager.h"
#include "page_manager.h"
#include "config.h"
#include "ui.h"
#include "input_manager.h"
#include "font_manager.h"
#include "config.h"

#define X_GAP   5
#define Y_GAP   5

static button_obj_t g_buttons[ITEM_CFG_MAX_NUM];
static int g_button_count;

static int main_page_press_button(struct _button_obj_t *button, display_buff_t *display_buff, input_event_t *input_event)
{
    unsigned int color = BUTTON_DEFAULT_COLOR;
    char name[128] = {0};
    char state[128] = {0};
    char *command_state[3] = {"ok", "err", "precent"};
    int comman_state_index = 0;
    char command[100];
    item_cfg_t *item;

    if (input_event->type == INPUT_TYPE_TOUCHSCREEN) {
        //分辨是否能被点击
        if (!get_item_cfg_by_name(button->name)->can_be_touched) {
            return -1;
        }
        if (input_event->pressure > 0) {
            button->state = !button->state; 
        }
        
        if (button->state) {
            color = BUTTON_PRESSED_COLOR;
            comman_state_index = 0;
        } else {
            color = BUTTON_DEFAULT_COLOR;
            comman_state_index = 1;
        }
    } else if (input_event->type == INPUT_TYPE_NET) {
        sscanf(input_event->str, "%s %s", name, state);
        if (strcmp(state, "ok") == 0) {
            button->state = BUTTON_STATE_PRESSED;
            color = BUTTON_PRESSED_COLOR;
            comman_state_index = 0;
        } else if (strcmp(state, "err") == 0) {
            button->state = BUTTON_STATE_DEFAULT;
            color = BUTTON_DEFAULT_COLOR;
            comman_state_index = 1;
        } else if (state[0] > '0' && state[0] < '9') {
            button->state = BUTTON_STATE_PRECENT;
            color = BUTTON_PRECENT_COLOR;
            comman_state_index = 2;
        } else {
            printf("main_page_press_button -> err net msg.\n");
            return -1;
        }
    } else {
        printf("main_page_press_button -> button type err.\n");
        return -1;
    }

    //绘制方框底色
    draw_region(&button->region, color);

    //居中显示按键内容
    set_font_size(button->font_size);
    draw_text_in_region_central(button->name, button->region, BUTTON_TEXT_COLOR);

    //flush buffer
    flush_display_buff(&button->region, display_buff);

    item = get_item_cfg_by_name(button->name);
    if (item->command[0] != '\0') {
        sprintf(command, "%s %s", item->command, command_state[comman_state_index]);
        system(command);
    }
    
    return 0;
}

static int get_fontsize_for_all_button(void)
{
    region_cartesian_t region_car;
    int i = 0;
    int max_len = 0;
    int max_index = 0;
    int len;
    float k, kx, ky;

    for (i = 0; i != g_button_count; ++i) {
        len = strlen(g_buttons[i].name);
        if (len > max_len) {
            max_len = len;
            max_index = i;
        }
    }

    set_font_size(100);
    get_string_region_cartesian(g_buttons[max_index].name, &region_car);

    //把文字外框缩放为button外框
    kx = (float)g_buttons[max_index].region.width / region_car.width;
    ky = (float)g_buttons[max_index].region.hight / region_car.hight;

    k = (kx > ky) ? ky : kx;

    //乘0.8防止文字靠近方框边缘
    return k * 100 * 0.8;
}

static void generate_ui(void)
{
    display_buff_t *display_buff;
    int button_width, button_hight;
    int n_per_line;
    int rows;
    int start_x, start_y;
    int row, col;
    int i = 0;
    int font_size;

    g_button_count = get_item_count();

    display_buff = get_display_buff();

    button_width = sqrt(display_buff->x_res * display_buff->y_res / g_button_count / 0.618);

    n_per_line = display_buff->x_res / button_width;
    //保证不越界，缩小width
    n_per_line++;
    button_width = display_buff->x_res /  n_per_line;
    button_hight = button_width * 0.618;

    rows = g_button_count / n_per_line;
    if ((g_button_count % n_per_line) > 0) {
        rows++;
    }

    //居中显示
    start_x = (display_buff->x_res - button_width * n_per_line) / 2;
    start_y = (display_buff->y_res - button_hight * rows) / 2;

    for (row = 0; row != rows; ++row) {
        if (row != 0) {
            start_y = start_y + button_hight;
        }
        
        for (col = 0; (col != n_per_line) && (i < g_button_count); ++col) {
            g_buttons[i].region.x_res = start_x + (col * button_width);
            g_buttons[i].region.y_res = start_y;
            g_buttons[i].region.width = button_width - X_GAP;
            g_buttons[i].region.hight = button_hight - Y_GAP;

            button_init(get_item_cfg_by_index(i)->name, &g_buttons[i], NULL, NULL, main_page_press_button);
            i++;
        }
    }

    font_size = get_fontsize_for_all_button();
    
    for (i = 0; i != g_button_count; ++i) {
        g_buttons[i].font_size = font_size;
        g_buttons[i].draw_button(&g_buttons[i], display_buff);
    }

}

int is_touch_point_region(int x, int y, region_t region) 
{
    if ((x > region.x_res) && (x < region.x_res + region.width) &&\
        (y > region.y_res) && (y < region.y_res + region.hight)) {
            return 0;
        }
    return -1;
}

button_obj_t *get_button_by_name(char *name)
{
    int i = 0;

    for (i = 0; i != g_button_count; ++i) {
        if (strcmp(name, g_buttons[i].name) == 0) {
            return &g_buttons[i];
        }
    }

    return NULL;
}

button_obj_t *get_button_by_input_event(input_event_t input_event)
{
    int i = 0;
    char name[100] = {0};

    if (input_event.type == INPUT_TYPE_TOUCHSCREEN) {
        for (i = 0; i != g_button_count; ++i) {
            if (!is_touch_point_region(input_event.x, input_event.y, g_buttons[i].region)) {
                printf("is button : %d\n", i);
                return &g_buttons[i];
            }
        }
    } else if (input_event.type == INPUT_TYPE_NET) {
        sscanf(input_event.str, "%s", name);
        return get_button_by_name(name);
    } else {  
        printf("input event type err.\n");
    }

    return NULL;
}

void main_page_run(void *parameter)
{
    int ret;
    input_event_t input_event;
    button_obj_t *button = NULL;
    display_buff_t *display_buff;

    display_buff = get_display_buff();
    if (display_buff == NULL) {
        printf("get display_buff err.\n");
    }

    /* 读取配置文件 */
    ret = prase_config_file();
    if (ret) {
        printf("main_page_run -> prase_config_file err\n");
        return;
    }

    /* 根据配置文件生成界面 */
    generate_ui();
    printf("success generate\n");

    while (1) {
        /* 读取输入事件 */
        get_input_event(&input_event);

        /* 根据输入事件，找到对应按钮 */
        button = get_button_by_input_event(input_event);
        if (ret) {
            printf("main_page_run -> get_button_by_input_event err\n");
        }

        /* 执行对应操作 */
        if (button) {
            button->press_button(button, display_buff, &input_event);
            printf("take button->press_button\n");
        }
        
    }
}

page_action_t main_page = {
    .name = "main",
    .run = main_page_run,
};

void register_main_page(void)
{
    register_page(&main_page);
}