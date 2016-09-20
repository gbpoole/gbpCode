#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void free_scenes(scene_info **scene){
  SID_log("Freeing scene...",SID_LOG_OPEN);
  // If this scene has been allocated ...
  if((*scene)!=NULL){
    // ... free its linked list
    scene_info *current=(*scene);
    while(current!=NULL){
      scene_info *next=current->next;
      for(int i_frame=0;i_frame<(*scene)->n_frames;i_frame++)
         free_perspective(&(current->perspectives[i_frame]));
      SID_free(SID_FARG current->perspectives);
      SID_free(SID_FARG current);
      current=next;
    }
  }
  SID_log("Done.",SID_LOG_CLOSE);
}

