#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

// This routine is not efficient, but it doesn't need to be
int add_render_depth_local(render_info *render,const char *id,double FOV_x,double FOV_y,double x,double y,double d_depth,double f_stretch);
int add_render_depth_local(render_info *render,const char *id,double FOV_x,double FOV_y,double x,double y,double d_depth,double f_stretch){
   int r_val=TRUE;

   render->camera->n_depth++;
   render->camera->depth_array           =(double *)SID_realloc(render->camera->depth_array,           sizeof(double)*render->camera->n_depth);
   render->camera->depth_array_identifier=(char  **)SID_realloc(render->camera->depth_array_identifier,sizeof(char *)*render->camera->n_depth);
   render->camera->depth_array_x         =(double *)SID_realloc(render->camera->depth_array_x,         sizeof(double)*render->camera->n_depth);
   render->camera->depth_array_y         =(double *)SID_realloc(render->camera->depth_array_y,         sizeof(double)*render->camera->n_depth);
   render->camera->depth_array_FOV_x     =(double *)SID_realloc(render->camera->depth_array_FOV_x,     sizeof(double)*render->camera->n_depth);
   render->camera->depth_array_FOV_y     =(double *)SID_realloc(render->camera->depth_array_FOV_y,     sizeof(double)*render->camera->n_depth);
   render->camera->depth_array_f_stretch =(double *)SID_realloc(render->camera->depth_array_f_stretch, sizeof(double)*render->camera->n_depth);
   render->camera->depth_array          [render->camera->n_depth-1]=d_depth;
   render->camera->depth_array_x        [render->camera->n_depth-1]=x;
   render->camera->depth_array_y        [render->camera->n_depth-1]=y;
   render->camera->depth_array_FOV_x    [render->camera->n_depth-1]=FOV_x;
   render->camera->depth_array_FOV_y    [render->camera->n_depth-1]=FOV_y;
   render->camera->depth_array_f_stretch[render->camera->n_depth-1]=f_stretch;

   render->camera->depth_array_identifier[render->camera->n_depth-1]=(char *)SID_malloc(sizeof(char)*GBPRENDER_DEPTH_ARRAY_ID_SIZE);
   strcpy(render->camera->depth_array_identifier[render->camera->n_depth-1],id);

   return(r_val);
}

// This code must only be called after the perspective has been finalized and
//    updated for the camera because we need things like d_o
int set_camera_depths(render_info *render,int stereo_offset_dir){
  int r_val=TRUE;

  // Set a dummy value for now
  int mode = TRUE;

  // Apply transformation
  double FOV_x_object_plane = 0.;
  double FOV_y_object_plane = 0.;
  double d_o                = 0.;
  double x_o                = 0.;
  double y_o                = 0.;
  double z_o                = 0.;
  double x_c                = 0.;
  double y_c                = 0.;
  double z_c                = 0.;
  double x_hat              = 0.;
  double y_hat              = 0.;
  double z_hat              = 0.;
  double theta              = 0.;
  double theta_roll         = 0.;
  compute_perspective_transformation(render,
                                     stereo_offset_dir,
                                     &FOV_x_object_plane,
                                     &FOV_y_object_plane,
                                     &d_o, // Includes f_image_plane correction
                                     &x_o,
                                     &y_o,
                                     &z_o,
                                     &x_c,
                                     &y_c,
                                     &z_c,
                                     &x_hat,
                                     &y_hat,
                                     &z_hat,
                                     &theta,
                                     &theta_roll);

  // Set stereo offset
  double stereo_offset = (float)stereo_offset_dir*render->camera->perspective->stereo_offset;

  // Save the old count so we can check the non-initializing calls below
  int n_depth_old=render->camera->n_depth;

  // Reset list to zero
  free_camera_depths(render->camera);

  // !!! This must always be present !!!
  add_render_depth_local(render,"full_frame",1.,1.,0.,0.,0.,0.);

  // Add the object plane (if requested)
  if(check_mode_for_flag(mode,TRUE)){
     float f_stretch=compute_f_stretch(render->camera->perspective->d_image_plane,render->camera->perspective->d_o,check_mode_for_flag(render->camera->camera_mode,CAMERA_PLANE_PARALLEL));
     add_render_depth_local(render,
             "p_object_plane",
             render->camera->perspective->FOV_x_object_plane,
             render->camera->perspective->FOV_y_object_plane,
             stereo_offset,
             0.,
             render->camera->perspective->d_o,
             f_stretch);
  }

  // Add the image plane (if requested)
  if(check_mode_for_flag(mode,FALSE))
     add_render_depth_local(render,
             "p_image_plane",
             render->camera->perspective->FOV_x_image_plane,
             render->camera->perspective->FOV_y_image_plane,
             stereo_offset, 
             0.,
             render->camera->perspective->d_image_plane,
             1.); // f_stretch=1 at image plane

  // Add marked objects (if requested)
  if(check_mode_for_flag(mode,TRUE)){
     mark_arg_info *current_arg=render->mark_arg_first;
     int i_arg =0;
     int i_mark=0;
     while(current_arg!=NULL){
         if(!strcmp(current_arg->type,"group_id") || !strcmp(current_arg->type,"subgroup_id")){
             char mark_id[GBPRENDER_DEPTH_ARRAY_ID_SIZE];
             if(!strcmp(current_arg->type,"group_id"))
                sprintf(mark_id,"group_%d",current_arg->ival[0]);
             else if(!strcmp(current_arg->type,"subgroup_id"))
                sprintf(mark_id,"subgroup_%d",current_arg->ival[0]);
             else
                SID_trap_error("Invalid option.",ERROR_LOGIC);
             float x_m=0.;
             float y_m=0.;
             float z_m=0.;
             if(render->camera->flag_velocity_space){
                x_m=(float)render->mark_properties[i_mark].velocity_COM[0];
                y_m=(float)render->mark_properties[i_mark].velocity_COM[1];
                z_m=(float)render->mark_properties[i_mark].velocity_COM[2];
             }
             else{
                x_m=(float)render->mark_properties[i_mark].position_COM[0];
                y_m=(float)render->mark_properties[i_mark].position_COM[1];
                z_m=(float)render->mark_properties[i_mark].position_COM[2];
             }
             transform_particle(&x_m,
                                &y_m,
                                &z_m,
                                render->camera->perspective->p_o[0],
                                render->camera->perspective->p_o[1],
                                render->camera->perspective->p_o[2],
                                x_hat, 
                                y_hat, 
                                z_hat, 
                                render->camera->perspective->d_o,
                                stereo_offset,
                                theta,
                                theta_roll, 
                                render->box_size, 
                                render->camera->perspective->time, 
                                render->camera->perspective->focus_shift_x,
                                render->camera->perspective->focus_shift_y,
                                render->flag_comoving, 
                                render->flag_force_periodic); 
             float f_stretch=compute_f_stretch(render->camera->perspective->d_image_plane,z_m,check_mode_for_flag(render->camera->camera_mode,CAMERA_PLANE_PARALLEL));
             add_render_depth_local(render,
                                    mark_id,
                                    render->camera->perspective->FOV_x_image_plane/f_stretch,
                                    render->camera->perspective->FOV_y_image_plane/f_stretch,
                                    x_m+stereo_offset,
                                    y_m,
                                    z_m,
                                    f_stretch);
             i_mark++;
         }
         current_arg=current_arg->next;
         i_arg++;
     }
  }

  // Check that the count never changes
  if(!render->camera->flag_depth_init)
      if(render->camera->n_depth!=n_depth_old)
          SID_trap_error("The camera depth counter has changed (ie. %d!=%d)",ERROR_LOGIC,render->camera->n_depth,n_depth_old);
  render->camera->flag_depth_init=FALSE;

  return(r_val);
}

