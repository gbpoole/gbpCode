#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void set_frame(camera_info *camera) {
    int          i_x, i_y, i_pixel;
    int          i_image;
    double       image_min, image_max;
    double       image_range;
    char         pixel_value;
    double       brightness;
    image_info **image_RGB         = NULL;
    image_info **image_Y           = NULL;
    image_info **image_RGBY        = NULL;
    image_info **image_RY          = NULL;
    image_info **image_GY          = NULL;
    image_info **image_BY          = NULL;
    image_info **image             = NULL;
    image_info **image_RGBY_MARKED = NULL;
    double *     values;

    // Loop over each set of images
    for(i_image = 0; i_image < 3; i_image++) {
        int flag_log_RGB = check_mode_for_flag(camera->camera_mode, CAMERA_LOG_RGB);
        int flag_log_Y   = check_mode_for_flag(camera->camera_mode, CAMERA_LOG_Y);
        switch(i_image) {
            case 0: // left image of a stereo pair
                image_RGB         = camera->image_RGB_left;
                image_Y           = camera->image_Y_left;
                image_RGBY        = camera->image_RGBY_left;
                image_RY          = camera->image_RY_left;
                image_GY          = camera->image_GY_left;
                image_BY          = camera->image_BY_left;
                image_RGBY_MARKED = camera->image_RGBY_MARKED_left;
                break;
            case 1: // right image of a stereo pair
                image_RGB         = camera->image_RGB_right;
                image_Y           = camera->image_Y_right;
                image_RGBY        = camera->image_RGBY_right;
                image_RY          = camera->image_RY_right;
                image_GY          = camera->image_GY_right;
                image_BY          = camera->image_BY_right;
                image_RGBY_MARKED = camera->image_RGBY_MARKED_right;
                break;
            case 2: // mono image
                image_RGB         = camera->image_RGB;
                image_Y           = camera->image_Y;
                image_RGBY        = camera->image_RGBY;
                image_RY          = camera->image_RY;
                image_GY          = camera->image_GY;
                image_BY          = camera->image_BY;
                image_RGBY_MARKED = camera->image_RGBY_MARKED;
                break;
        }

        // Set RGB & Y images
        if(image_RGBY != NULL) {
            for(int i_depth = 0; i_depth < camera->n_depth; i_depth++) {
                set_image_RGB(image_RGB[i_depth], camera->RGB_range[0], camera->RGB_range[1], flag_log_RGB);
                set_image_RGB(image_Y[i_depth], camera->Y_range[0], camera->Y_range[1], flag_log_Y);
                set_image_RGBY(image_RGBY[i_depth],
                               image_RGB[i_depth],
                               image_Y[i_depth],
                               camera->RGB_range[0],
                               camera->RGB_range[1],
                               camera->Y_range[0],
                               camera->Y_range[1],
                               flag_log_RGB,
                               flag_log_Y);
            }
        }
        if(image_RGBY_MARKED != NULL) {
            for(int i_depth = 0; i_depth < camera->n_depth; i_depth++) {
                set_image_RGB(image_RY[i_depth], 0., 1., GBP_FALSE);
                set_image_RGB(image_GY[i_depth], 0., 1., GBP_FALSE);
                set_image_RGB(image_BY[i_depth], 0., 1., GBP_FALSE);
                set_image_RGBY_MARKED(image_RGBY_MARKED[i_depth],
                                      image_RY[i_depth],
                                      image_GY[i_depth],
                                      image_BY[i_depth],
                                      image_Y[i_depth],
                                      camera->Y_range[0],
                                      camera->Y_range[1],
                                      flag_log_Y);
            }
        }
    }
}
