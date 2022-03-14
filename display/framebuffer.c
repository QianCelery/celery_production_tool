/*
 * @Author: Celery
 * @Date: 2022-03-13 21:12:28
 * @LastEditTime: 2022-03-14 11:04:46
 * @LastEditors: Celery
 * @Description: lcd framebuffer各项操作函数实现
 * @FilePath: \celery_production_tool\display\framebuffer.c
 * 
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>

#include "display_manager.h"

static int fd_fb;   //file_descriptor
static struct fb_var_screeninfo var;
static unsigned int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

/**
 * @description: framebuffer初始化
 * @param {none}
 * @return {0:成功 -1:失败}
 */
static int fb_dev_init(void)
{
    fd_fb = open("/dev/fb0", O_RDWR);
    if(fd_fb == -1)
    {
        printf("Can't open file:/dev/fb0.");
        return -1;
    }

    if(ioctl(fd_fb, FBIOGET_VSCREENINFO, &var) == -1)
    {
        printf("Can't get var of fd_fb.");
        return -1;
    }
    
    line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if(fb_base == (unsigned char *)MAP_FAILED)
    {
        printf("Can't creat new mapping in the fd_fb.");
        return -1;
    }

    return 0;
}

/**
 * @description: framebuffer退出函数
 * @param {none}
 * @return {*none}
 */
static void fb_dev_exit(void)
{
    munmap(fb_base, screen_size);

    close(fd_fb);
}

/**
 * @description: 以display_buff_t的形式获取显示缓存
 * @param {display_buff_t} *display_buf
 * @return {0:获取成功}
 */
static int fb_get_buffer(display_buff_t *display_buf)
{
    display_buf->x_res = var.xres;
    display_buf->y_res = var.yres;
    display_buf->bpp   = var.bits_per_pixel;
    display_buf->buff  = fb_base;

    return 0;
}

/**
 * @description: 将region区域刷新到缓存,使用lcd无需此操作
 * @param {region_t} *region
 * @param {display_buff_t} *display_buf
 * @return {0:成功}
 */
static int fb_flush_region(region_t *region, display_buff_t *display_buf)
{
    return 0;
}

display_operations_t fb_opr = {
    .name       = "fb",
    .dev_init   = fb_dev_init,
    .dev_exit   = fb_dev_exit,
    .get_buffer = fb_get_buffer,
    .flush_region = fb_flush_region,
};

/**
 * @description: 注册fb_opr
 * @param {display_operations_t} *display_oprs
 * @return {*}
 */
void framebuffer_register(void)
{
    display_register(&fb_opr);
}