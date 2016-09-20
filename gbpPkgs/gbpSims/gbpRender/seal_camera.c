#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void seal_camera(render_info *render){

  SID_log("Sealing camera...",SID_LOG_OPEN);

  // Decide if we're producing a stereo image based on stereo_factor
  if(render->camera->stereo_ratio>0.)
     render->camera->camera_mode|=CAMERA_STEREO;

  // Initialize the perspective information for this camera
  copy_perspective(render->first_scene->perspectives[0],render->camera->perspective);

  // Decide how many depths to split the output into
  render->camera->n_depth=3+render->n_mark_properties;

  // Initialize image buffers 
  if(check_mode_for_flag(render->camera->camera_mode,CAMERA_STEREO)){
    // LEFT
    int flag_images_defined=FALSE;
    if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_1CHANNEL)){
       render->camera->image_RGB_left =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_Y_left   =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_RGBY_left=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
          init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGB_left[i_depth]));
          init_image(render->camera->width,render->camera->height,"greyscale",                 &(render->camera->image_Y_left[i_depth]));
          init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGBY_left[i_depth]));
       }
       flag_images_defined=TRUE;
    }
    if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_MARKED)){
       render->camera->image_RY_left         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_GY_left         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_BY_left         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_RGBY_MARKED_left=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
          init_image(render->camera->width,render->camera->height,"red",      &(render->camera->image_RY_left[i_depth]));
          init_image(render->camera->width,render->camera->height,"green",    &(render->camera->image_GY_left[i_depth]));
          init_image(render->camera->width,render->camera->height,"blue",     &(render->camera->image_BY_left[i_depth]));
          init_image(render->camera->width,render->camera->height,"greyscale",&(render->camera->image_RGBY_MARKED_left[i_depth])); // use greyscale as a dummy
       }
       flag_images_defined=TRUE;
    }
    if(!flag_images_defined)
       SID_trap_error("Invalid camera RGB mode (%d) specified in seal_camera().",ERROR_LOGIC,render->camera->RGB_mode);
    // RIGHT
    flag_images_defined=FALSE;
    if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_1CHANNEL)){
       render->camera->image_RGB_right =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_Y_right   =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_RGBY_right=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
          init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGB_right[i_depth]));
          init_image(render->camera->width,render->camera->height,"greyscale",                 &(render->camera->image_Y_right[i_depth]));
          init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGBY_right[i_depth]));
       }
       flag_images_defined=TRUE;
    }
    if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_MARKED)){
       render->camera->image_RY_right         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_GY_right         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_BY_right         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       render->camera->image_RGBY_MARKED_right=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
       for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
           init_image(render->camera->width,render->camera->height,"red",      &(render->camera->image_RY_right[i_depth]));
           init_image(render->camera->width,render->camera->height,"green",    &(render->camera->image_GY_right[i_depth]));
           init_image(render->camera->width,render->camera->height,"blue",     &(render->camera->image_BY_right[i_depth]));
           init_image(render->camera->width,render->camera->height,"greyscale",&(render->camera->image_RGBY_MARKED_right[i_depth])); // use greyscale as a dummy
       }
       flag_images_defined=TRUE;
    }
    if(!flag_images_defined)
       SID_trap_error("Invalid camera RGB mode (%d) specified in seal_camera().",ERROR_LOGIC,render->camera->RGB_mode);
  }
  else{
     int flag_images_defined=FALSE;
     if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_1CHANNEL)){
        render->camera->image_RGB =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        render->camera->image_Y   =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        render->camera->image_RGBY=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
           init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGB[i_depth]));
           init_image(render->camera->width,render->camera->height,"greyscale",                 &(render->camera->image_Y[i_depth]));
           init_image(render->camera->width,render->camera->height,render->camera->colour_table,&(render->camera->image_RGBY[i_depth]));
        }
        flag_images_defined=TRUE;
     }
     if(check_mode_for_flag(render->camera->RGB_mode,CAMERA_RGB_MODE_MARKED)){
        render->camera->image_RY         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        render->camera->image_GY         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        render->camera->image_BY         =(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        render->camera->image_RGBY_MARKED=(image_info **)SID_malloc(sizeof(image_info *)*render->camera->n_depth);
        for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
           init_image(render->camera->width,render->camera->height,"red",      &(render->camera->image_RY[i_depth]));
           init_image(render->camera->width,render->camera->height,"green",    &(render->camera->image_GY[i_depth]));
           init_image(render->camera->width,render->camera->height,"blue",     &(render->camera->image_BY[i_depth]));
           init_image(render->camera->width,render->camera->height,"greyscale",&(render->camera->image_RGBY_MARKED[i_depth])); // use greyscale as a dummy
        }
        flag_images_defined=TRUE;
     }
     if(!flag_images_defined)
        SID_trap_error("Invalid camera RGB mode (%d) specified in seal_camera().",ERROR_LOGIC,render->camera->RGB_mode);
  }

  SID_log("Done.",SID_LOG_CLOSE);
}

