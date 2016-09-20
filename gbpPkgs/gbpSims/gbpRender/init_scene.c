#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void init_scene(scene_info **scene,int n_frames){
  SID_log("Initializing scene...",SID_LOG_OPEN);
  if(n_frames<=0)
     SID_trap_error("Invalid number of frames (%d) requested for scene.",ERROR_LOGIC,n_frames);
  (*scene)                 =(scene_info *)SID_malloc(sizeof(scene_info));
  (*scene)->n_frames       =n_frames;
  (*scene)->perspectives   =(perspective_info **)SID_malloc(sizeof(perspective_info *)*n_frames);
  for(int i_frame=0;i_frame<n_frames;i_frame++)
     init_perspective(&((*scene)->perspectives[i_frame]));
  (*scene)->sealed         =FALSE;
  (*scene)->next           =NULL;
  (*scene)->flag_time_set  =FALSE;
  (*scene)->flag_p_o_set   =FALSE;
  (*scene)->flag_radius_set=FALSE;
  SID_log("Done.",SID_LOG_CLOSE);
}

