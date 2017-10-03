#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void free_camera(camera_info **camera){
  SID_log("Freeing camera...",SID_LOG_OPEN);
  free_perspective(&((*camera)->perspective));
  SID_free(SID_FARG (*camera)->mask_RGB);
  SID_free(SID_FARG (*camera)->mask_Y);
  SID_free(SID_FARG (*camera)->mask_RGBY);
  SID_free(SID_FARG (*camera)->mask_RGBY_MARKED);
  for(int i_depth=0;i_depth<(*camera)->n_depth_alloc;i_depth++){
     if((*camera)->image_RGB!=NULL)
        free_image(&((*camera)->image_RGB[i_depth]));
     if((*camera)->image_RGB_left!=NULL)
        free_image(&((*camera)->image_RGB_left[i_depth]));
     if((*camera)->image_RGB_right!=NULL)
        free_image(&((*camera)->image_RGB_right[i_depth]));
     if((*camera)->image_Y!=NULL)
        free_image(&((*camera)->image_Y[i_depth]));
     if((*camera)->image_Y_left!=NULL)
        free_image(&((*camera)->image_Y_left[i_depth]));
     if((*camera)->image_Y_right!=NULL)
        free_image(&((*camera)->image_Y_right[i_depth]));
     if((*camera)->image_RGBY!=NULL)
        free_image(&((*camera)->image_RGBY[i_depth]));
     if((*camera)->image_RGBY_left!=NULL)
        free_image(&((*camera)->image_RGBY_left[i_depth]));
     if((*camera)->image_RGBY_right!=NULL)
        free_image(&((*camera)->image_RGBY_right[i_depth]));
     if((*camera)->image_RY!=NULL)
        free_image(&((*camera)->image_RY[i_depth]));
     if((*camera)->image_RY_left!=NULL)
        free_image(&((*camera)->image_RY_left[i_depth]));
     if((*camera)->image_RY_right!=NULL)
        free_image(&((*camera)->image_RY_right[i_depth]));
     if((*camera)->image_GY!=NULL)
        free_image(&((*camera)->image_GY[i_depth]));
     if((*camera)->image_GY_left!=NULL)
        free_image(&((*camera)->image_GY_left[i_depth]));
     if((*camera)->image_GY_right!=NULL)
        free_image(&((*camera)->image_GY_right[i_depth]));
     if((*camera)->image_BY!=NULL)
        free_image(&((*camera)->image_BY[i_depth]));
     if((*camera)->image_BY_left!=NULL)
        free_image(&((*camera)->image_BY_left[i_depth]));
     if((*camera)->image_BY_right!=NULL)
        free_image(&((*camera)->image_BY_right[i_depth]));
     if((*camera)->image_RGBY_MARKED!=NULL)
        free_image(&((*camera)->image_RGBY_MARKED[i_depth]));
     if((*camera)->image_RGBY_MARKED_left!=NULL)
        free_image(&((*camera)->image_RGBY_MARKED_left[i_depth]));
     if((*camera)->image_RGBY_MARKED_right!=NULL)
        free_image(&((*camera)->image_RGBY_MARKED_right[i_depth]));
  }
  SID_free(SID_FARG (*camera)->image_RGB);
  SID_free(SID_FARG (*camera)->image_RGB_left);
  SID_free(SID_FARG (*camera)->image_RGB_right);
  SID_free(SID_FARG (*camera)->image_Y);
  SID_free(SID_FARG (*camera)->image_Y_left);
  SID_free(SID_FARG (*camera)->image_Y_right);
  SID_free(SID_FARG (*camera)->image_RGBY);
  SID_free(SID_FARG (*camera)->image_RGBY_left);
  SID_free(SID_FARG (*camera)->image_RGBY_right);
  SID_free(SID_FARG (*camera)->image_RY);
  SID_free(SID_FARG (*camera)->image_RY_left);
  SID_free(SID_FARG (*camera)->image_RY_right);
  SID_free(SID_FARG (*camera)->image_GY);
  SID_free(SID_FARG (*camera)->image_GY_left);
  SID_free(SID_FARG (*camera)->image_GY_right);
  SID_free(SID_FARG (*camera)->image_BY);
  SID_free(SID_FARG (*camera)->image_BY_left);
  SID_free(SID_FARG (*camera)->image_BY_right);
  SID_free(SID_FARG (*camera)->image_RGBY_MARKED);
  SID_free(SID_FARG (*camera)->image_RGBY_MARKED_left);
  SID_free(SID_FARG (*camera)->image_RGBY_MARKED_right);
  SID_free(SID_FARG (*camera)->mask_RGB_left);
  SID_free(SID_FARG (*camera)->mask_Y_left);
  SID_free(SID_FARG (*camera)->mask_RGBY_left);
  SID_free(SID_FARG (*camera)->mask_RGBY_MARKED_left);
  SID_free(SID_FARG (*camera)->mask_RGB_right);
  SID_free(SID_FARG (*camera)->mask_Y_right);
  SID_free(SID_FARG (*camera)->mask_RGBY_right);
  SID_free(SID_FARG (*camera)->mask_RGBY_MARKED_right);
  if((*camera)->RGB_gamma!=NULL)
    free_interpolate(SID_FARG (*camera)->RGB_gamma,NULL);
  if((*camera)->transfer_list!=NULL)
    ADaPS_free(SID_FARG (*camera)->transfer_list);
  if((*camera)->Y_gamma!=NULL)
    free_interpolate(SID_FARG (*camera)->Y_gamma,NULL);

  // Free camera depth information
  free_camera_depths(*camera);

  SID_free((void **)camera);
  SID_log("Done.",SID_LOG_CLOSE);
}

