#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void set_frame(camera_info *camera){
  int         i_x,i_y,i_pixel;
  int         i_image;
  double      image_min,image_max;
  double      image_range;
  char        pixel_value;
  double      brightness;
  image_info *image_RGB;
  image_info *image_Y;
  image_info *image_Z;
  image_info *image_RGBY;
  image_info *image_RY;
  image_info *image_GY;
  image_info *image_BY;
  image_info *image;
  image_info *image_RGBY_MARKED;
  double     *values;

  // Loop over each set of images
  for(i_image=0;i_image<3;i_image++){
    int flag_log_RGB=check_mode_for_flag(camera->camera_mode,CAMERA_LOG_RGB);
    int flag_log_Y  =check_mode_for_flag(camera->camera_mode,CAMERA_LOG_Y);
    switch(i_image){
    case 0: // left image of a stereo pair
      image_RGB          =camera->image_RGB_left;
      image_Y            =camera->image_Y_left;
      image_Z            =camera->image_Z_left;
      image_RGBY         =camera->image_RGBY_left;
      image_RY           =camera->image_RY_left;
      image_GY           =camera->image_GY_left;
      image_BY           =camera->image_BY_left;
      image_RGBY_MARKED=camera->image_RGBY_MARKED_left;
      break;
    case 1: // right image of a stereo pair
      image_RGB          =camera->image_RGB_right;
      image_Y            =camera->image_Y_right;
      image_Z            =camera->image_Z_right;
      image_RGBY         =camera->image_RGBY_right;
      image_RY           =camera->image_RY_right;
      image_GY           =camera->image_GY_right;
      image_BY           =camera->image_BY_right;
      image_RGBY_MARKED=camera->image_RGBY_MARKED_right;
      break;
    case 2: // mono image
      image_RGB          =camera->image_RGB;
      image_Y            =camera->image_Y;
      image_Z            =camera->image_Z;
      image_RGBY         =camera->image_RGBY;
      image_RY           =camera->image_RY;
      image_GY           =camera->image_GY;
      image_BY           =camera->image_BY;
      image_RGBY_MARKED=camera->image_RGBY_MARKED;
      break;
    }

    // Set RGB & Y images
    if(image_RGBY!=NULL){
       set_image_RGB(image_RGB,camera->RGB_range[0],camera->RGB_range[1],flag_log_RGB);
       set_image_RGB(image_Y,camera->Y_range[0],camera->Y_range[1],flag_log_Y);
       set_image_RGBY(image_RGBY,
                      image_RGB,
                      image_Y,
                      camera->RGB_range[0],
                      camera->RGB_range[1],
                      camera->Y_range[0],
                      camera->Y_range[1],
                      flag_log_RGB,flag_log_Y);
    }
    if(image_RGBY_MARKED!=NULL){
       set_image_RGB(image_RY,0.,1.,FALSE);
       set_image_RGB(image_GY,0.,1.,FALSE);
       set_image_RGB(image_BY,0.,1.,FALSE);
       set_image_RGBY_MARKED(image_RGBY_MARKED,
                               image_RY,
                               image_GY,
                               image_BY,
                               image_Y,
                               camera->Y_range[0],
                               camera->Y_range[1],
                               flag_log_Y);
    }

    // Set Z image
    if(image_Z!=NULL)
       set_image_RGB(image_Z,camera->Z_range[0],camera->Z_range[1],FALSE);
  }
}

