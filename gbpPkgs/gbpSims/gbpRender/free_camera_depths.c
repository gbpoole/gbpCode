#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

int free_camera_depths(camera_info *camera){
  int r_val=TRUE;

  SID_free(SID_FARG camera->depth_array_FOV_x);
  SID_free(SID_FARG camera->depth_array_FOV_y);
  SID_free(SID_FARG camera->depth_array);
  for(int i_depth=0;i_depth<camera->n_depth;i_depth++)
     SID_free(SID_FARG camera->depth_array_identifier[i_depth]);
  SID_free(SID_FARG camera->depth_array_identifier);
  SID_free(SID_FARG camera->depth_array_x);
  SID_free(SID_FARG camera->depth_array_y);
  camera->n_depth=0;

  return(r_val);
}

