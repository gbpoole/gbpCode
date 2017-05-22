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
int add_render_depth_local(render_info *render,double x,double y,double d_depth);
int add_render_depth_local(render_info *render,double x,double y,double d_depth){
   int r_val=TRUE;

   render->camera->n_depth++;
   render->camera->depth_array  =(double *)SID_realloc(render->camera->depth_array,  render->camera->n_depth);
   render->camera->depth_array_x=(double *)SID_realloc(render->camera->depth_array_x,render->camera->n_depth);
   render->camera->depth_array_y=(double *)SID_realloc(render->camera->depth_array_y,render->camera->n_depth);
   render->camera->depth_array  [render->camera->n_depth-1]=d_depth;
   render->camera->depth_array_x[render->camera->n_depth-1]=x;
   render->camera->depth_array_y[render->camera->n_depth-1]=y;

   return(r_val);
}

// This code must only be called after the perspective has been finalized and
//    updated for the camera because we need things like d_o
int set_camera_depths(render_info *render,int flag_stereo_offset){
  int r_val=TRUE;

  // Save the old count so we can check the non-initializing calls below
  int n_depth_old=render->camera->n_depth;

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
                                     flag_stereo_offset,
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

  // Reset list to zero
  render->camera->n_depth=0;
  SID_free(SID_FARG render->camera->depth_array);

  // This must always be present
  add_render_depth_local(render,0.,0.,0.);

  // Add the object plane (if requested)
  if(check_mode_for_flag(mode,TRUE))
     add_render_depth_local(render,0.,0.,render->camera->perspective->d_o);

  // Add the image plane (if requested)
  if(check_mode_for_flag(mode,FALSE))
     add_render_depth_local(render,0.,0.,render->camera->perspective->d_image_plane);

  // Add marked objects (if requested)
  if(check_mode_for_flag(mode,TRUE)){
     for(int i_mark=0;i_mark<render->n_mark_properties;i_mark++){
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
                            0.,// lateral motions don't affect z_m, so we ignore render->camera->perspective->stereo_offset,
                            theta,
                            theta_roll, 
                            render->box_size, 
                            render->camera->perspective->time, 
                            render->camera->perspective->focus_shift_x,
                            render->camera->perspective->focus_shift_y,
                            render->flag_comoving, 
                            render->flag_force_periodic); 
         add_render_depth_local(render,x_m,y_m,z_m);
     }
  }

  // Check that the count never changes
  if(!render->camera->flag_depth_init)
      if(render->camera->n_depth!=n_depth_old)
          SID_trap_error("The camera depth counter has changed (ie. %d!=%d)",ERROR_LOGIC,render->camera->n_depth,n_depth_old);
  render->camera->flag_depth_init=FALSE;

  return(r_val);
}

