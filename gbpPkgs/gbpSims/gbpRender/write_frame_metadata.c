#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void write_frame_metadata(render_info *render,int frame,const char *set_label){

  if(SID.I_am_Master){
     char  filename[MAX_FILENAME_LENGTH];
     FILE *fp_out=NULL;

     // Write render file
     sprintf(filename,"%s/meta_render_%s_%05d.txt",render->filename_out_dir,set_label,frame);
     fp_out=fopen(filename,"w");
     fprintf(fp_out,"%%n_interpolate          %d\n", render->n_interpolate);
     fprintf(fp_out,"%%n_frames               %d\n", render->n_frames);
     fprintf(fp_out,"%%filename_SSimPL_root   %s\n", render->filename_SSimPL_root);
     fprintf(fp_out,"%%filename_halos_version %s\n", render->filename_halos_version);
     fprintf(fp_out,"%%filename_trees_version %s\n", render->filename_trees_version);
     fprintf(fp_out,"%%snap_filename_root     %s\n", render->snap_filename_root);
     fprintf(fp_out,"%%mark_filename_root     %s\n", render->mark_filename_root);
     fprintf(fp_out,"%%smooth_filename_root   %s\n", render->smooth_filename_root);
     fprintf(fp_out,"%%snap_a_list_filename   %s\n", render->snap_a_list_filename);
     fprintf(fp_out,"%%flag_comoving          %d\n", render->flag_comoving);
     fprintf(fp_out,"%%flag_fade              %d\n", render->flag_fade);
     fprintf(fp_out,"%%alpha_fade             %le\n",render->alpha_fade);
     fprintf(fp_out,"%%flag_force_periodic    %d\n", render->flag_force_periodic);
     fprintf(fp_out,"%%flag_read_marked       %d\n", render->flag_read_marked);
     fprintf(fp_out,"%%flag_add_absorption    %d\n", render->flag_add_absorption);
     fprintf(fp_out,"%%f_absorption           %le\n",render->f_absorption);
     fprintf(fp_out,"%%w_mode                 %d\n", render->w_mode);
     fprintf(fp_out,"%%v_mode                 %d\n", render->v_mode);
     fclose(fp_out);

     // Write camera file
     sprintf(filename,"%s/meta_camera_%s_%05d.txt",render->filename_out_dir,set_label,frame);
     fp_out=fopen(filename,"w");
     camera_info *camera=render->camera;
     fprintf(fp_out,"%%camera_mode         %d\n",     camera->camera_mode);
     fprintf(fp_out,"%%colour_table        %s\n",     camera->colour_table);
     fprintf(fp_out,"%%flag_velocity_space %d\n",     camera->flag_velocity_space);
     fprintf(fp_out,"%%width               %d\n",     camera->width);
     fprintf(fp_out,"%%height              %d\n",     camera->height);
     fprintf(fp_out,"%%stereo_ratio        %le\n",    camera->stereo_ratio);
     fprintf(fp_out,"%%f_near_field        %le\n",    camera->f_near_field);
     fprintf(fp_out,"%%f_taper_field       %le\n",    camera->f_taper_field);
     fprintf(fp_out,"%%f_image_plane       %le\n",    camera->f_image_plane);
     fprintf(fp_out,"%%RGB_mode            %d\n",     camera->RGB_mode);
     fprintf(fp_out,"%%RGB_param           %s\n",     camera->RGB_param);
     fprintf(fp_out,"%%RGB_range           %le %le\n",camera->RGB_range[0],camera->RGB_range[1]);
     fprintf(fp_out,"%%Y_mode              %d\n",     camera->Y_mode);
     fprintf(fp_out,"%%Y_param             %s\n",     camera->Y_param);
     fprintf(fp_out,"%%Y_range             %le %le\n",camera->Y_range[0],camera->Y_range[1]);
     fclose(fp_out);

     // Write perspective entry
     sprintf(filename,"%s/meta_perspective_%s_%05d.txt",render->filename_out_dir,set_label,frame);
     perspective_info *perspective=camera->perspective;
     int snap_best;
     pick_best_snap(perspective->time,render->snap_a_list,render->n_snap_a_list,&snap_best,NULL);
     fp_out=fopen(filename,"w");
     fprintf(fp_out,"%%Frame             %d\n",   frame);
     fprintf(fp_out,"%%Snapshot          %d\n",   snap_best);
     fprintf(fp_out,"%%Expansion factor  %11.5le\n",perspective->time);
     fprintf(fp_out,"%%p_o_x             %11.5le\n",perspective->p_o[0]);
     fprintf(fp_out,"%%p_o_y             %11.5le\n",perspective->p_o[1]);
     fprintf(fp_out,"%%p_o_z             %11.5le\n",perspective->p_o[2]);
     fprintf(fp_out,"%%p_c_x             %11.5le\n",perspective->p_c[0]);
     fprintf(fp_out,"%%p_c_y             %11.5le\n",perspective->p_c[1]);
     fprintf(fp_out,"%%p_c_z             %11.5le\n",perspective->p_c[2]);
     fprintf(fp_out,"%%d_o               %11.5le\n",perspective->d_o);
     fprintf(fp_out,"%%theta             %11.5le\n",perspective->theta);
     fprintf(fp_out,"%%phi               %11.5le\n",perspective->phi);
     fprintf(fp_out,"%%FOV_x_image_plane %11.5le\n",perspective->FOV_x_image_plane);
     fprintf(fp_out,"%%FOV_y_image_plane %11.5le\n",perspective->FOV_y_image_plane);
     fprintf(fp_out,"%%focus_shift_x     %11.5le\n",perspective->focus_shift_x);
     fprintf(fp_out,"%%focus_shift_y     %11.5le\n",perspective->focus_shift_y);
     fclose(fp_out);

     // Write depths
     int i_column=1;
     sprintf(filename,"%s/meta_depths_%s_%05d.txt",render->filename_out_dir,set_label,frame);
     fp_out=fopen(filename,"w");
     fprintf(fp_out,"# Camera data about the image depths for frame %d\n",frame);
     fprintf(fp_out,"# Column (%02d): Depth\n",     i_column++);
     fprintf(fp_out,"#        (%02d): identifier\n",i_column++);
     fprintf(fp_out,"#        (%02d): d [Mpc/h]\n", i_column++);
     fprintf(fp_out,"#        (%02d): x [fraction of frame width from left]\n",  i_column++);
     fprintf(fp_out,"#        (%02d): y [fraction of frame width from bottom]\n",i_column++);
     for(int i_depth=0;i_depth<render->camera->n_depth;i_depth++){
         fprintf(fp_out,"%2d %-24s %le %le %le\n",
                 i_depth,
                 render->camera->depth_array[i_depth],
                 render->camera->depth_array_identifier[i_depth],
                     0.5+render->camera->depth_array_x[i_depth]/render->camera->depth_array_FOV_x[i_depth],  // fraction of frame from left
                 1.-(0.5+render->camera->depth_array_y[i_depth]/render->camera->depth_array_FOV_y[i_depth])); // fraction of frame from *bottom*
     }
     fclose(fp_out);
  } 
}

