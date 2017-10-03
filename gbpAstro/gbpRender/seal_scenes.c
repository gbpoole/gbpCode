#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void seal_scenes(render_info *render){

  // Seal each scene in turn
  camera_info *camera = render->camera;
  scene_info  *current_scene=render->first_scene;
  int          i_scene=1;
  while(current_scene!=NULL){
      if(!current_scene->sealed){
         // Set the ancillary data for each perspective in the scene
         SID_log("Sealing scene %d...",SID_LOG_OPEN,i_scene);
         for(int i_frame=0;i_frame<current_scene->n_frames;i_frame++){
            perspective_info *perspective_i=current_scene->perspectives[i_frame];

            // Set some important distances
            perspective_i->d_near_field =camera->f_near_field *perspective_i->d_o;
            perspective_i->d_taper_field=camera->f_taper_field*perspective_i->d_o;
            perspective_i->d_image_plane=camera->f_image_plane*perspective_i->d_o;

            // Set stereo offset
            perspective_i->stereo_offset=perspective_i->d_image_plane/camera->stereo_ratio;

            // Set Field of View (FOV) at the object/image planes
            int    nx  = camera->width;
            int    ny  = camera->height;
            double FOV = perspective_i->FOV;
            if(nx>=ny){
              perspective_i->FOV_y_object_plane=FOV;
              perspective_i->FOV_x_object_plane=perspective_i->FOV_y_object_plane*(double)nx/(double)ny;
            }
            else{
              perspective_i->FOV_x_object_plane=FOV;
              perspective_i->FOV_y_object_plane=perspective_i->FOV_x_object_plane*(double)nx/(double)ny;
            }
            perspective_i->FOV_x_image_plane=perspective_i->FOV_x_object_plane*camera->f_image_plane;
            perspective_i->FOV_y_image_plane=perspective_i->FOV_y_object_plane*camera->f_image_plane;
         }

         // Set the flag saying that this scene has been sealed
         current_scene->sealed=TRUE;
         SID_log("Done.",SID_LOG_CLOSE);
      }
      current_scene=current_scene->next;
  }
}

