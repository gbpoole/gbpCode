#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void parse_set_scene_quantity(scene_info *scene,const char *quantity,char *line, int i_word_in){
   char parameter[64];
   // Interpret command
   int i_word=i_word_in;
   if(!strcmp(quantity,"time")){
      double time_in=1.;
      grab_double(line,i_word++,&time_in);
      for(int i_frame=0;i_frame<scene->n_frames;i_frame++)
         scene->perspectives[i_frame]->time=time_in;
      scene->flag_time_set=TRUE;
   }
   else if(!strcmp(quantity,"p_o")){
      double p_o_in[3];
      grab_double(line,i_word++,&(p_o_in[0]));
      grab_double(line,i_word++,&(p_o_in[1]));
      grab_double(line,i_word++,&(p_o_in[2]));
      for(int i_frame=0;i_frame<scene->n_frames;i_frame++){
         scene->perspectives[i_frame]->p_o[0]=p_o_in[0];
         scene->perspectives[i_frame]->p_o[1]=p_o_in[1];
         scene->perspectives[i_frame]->p_o[2]=p_o_in[2];
      }
      scene->flag_p_o_set=TRUE;
      SID_log("p_o set to [%le,%le,%le] for all frames in scene.",SID_LOG_COMMENT,p_o_in[0],p_o_in[1],p_o_in[2]);
   }
   else if(!strcmp(quantity,"radius")){
      // Checn that p_o has been set
      if(!(scene->flag_p_o_set))
         SID_log("p_o not yet set when radius is set in line {%s}.",SID_LOG_COMMENT,line);
      double radius_in=1.;
      grab_double(line,i_word++,&radius_in);
      for(int i_frame=0;i_frame<scene->n_frames;i_frame++)
         scene->perspectives[i_frame]->radius=radius_in;
      scene->flag_radius_set=TRUE;
      // Set p_c values
      for(int i_frame=0;i_frame<scene->n_frames;i_frame++){
         perspective_info *perspective_i=scene->perspectives[i_frame];
         perspective_i->p_c[0]=perspective_i->p_o[0]+perspective_i->radius*cos(perspective_i->zeta)*sin(perspective_i->theta);
         perspective_i->p_c[1]=perspective_i->p_o[1]+perspective_i->radius*cos(perspective_i->zeta)*cos(perspective_i->theta);
         perspective_i->p_c[2]=perspective_i->p_o[2]+perspective_i->radius*sin(perspective_i->zeta);
      }
      SID_log("radius set to %le for all frames in scene.",SID_LOG_COMMENT,radius_in);
   }
   else if(!strcmp(quantity,"FOV")){
      double FOV_in=1.;
      grab_double(line,i_word++,&FOV_in);
      for(int i_frame=0;i_frame<scene->n_frames;i_frame++)
         scene->perspectives[i_frame]->FOV=FOV_in;
      SID_log("FOV set to %le for all frames in scene.",SID_LOG_COMMENT,FOV_in);
   }
   else if(!strcmp(quantity,"theta")){
     grab_word(line,i_word,parameter);
     if(!(scene->flag_p_o_set))
        SID_log("p_o not yet set when theta is set in line {%s}.",SID_LOG_COMMENT,line);
     if(!(scene->flag_radius_set))
        SID_log("radius not yet set when theta is set in line {%s}.",SID_LOG_COMMENT,line);
     if(!strcmp(parameter,"evolve")){i_word++;
        grab_word(line,i_word,parameter);
        if(!strcmp(parameter,"with_frame")){i_word++;
           int n_params=count_words(line)-i_word+1;
           if(n_params<=1)
              SID_trap_error("Too-few evolution parameters passed in line {%s}.",ERROR_LOGIC,line);
           if(n_params==2){
              double val_min,val_max,dval;
              grab_double(line,i_word++,&val_min);  
              grab_double(line,i_word++,&val_max);
              dval=(val_max-val_min)/(double)(scene->n_frames-1);
              for(int i_frame=0;i_frame<scene->n_frames;i_frame++){
                 if(i_frame==0)
                    scene->perspectives[i_frame]->theta=val_min;
                 else if(i_frame==(scene->n_frames-1))
                    scene->perspectives[i_frame]->theta=val_max;
                 else
                    scene->perspectives[i_frame]->theta=val_min+dval*(double)i_frame;
              }
           }
           else
              SID_trap_error("Invalid evolution parameters passed in line {%s}.",ERROR_LOGIC,line);
        }
        else
           SID_trap_error("Invalid evolution parameters passed in line {%s}.",ERROR_LOGIC,line);
     }
     else{
        double theta_in=0.;
        grab_double(line,i_word++,&theta_in);  
        for(int i_frame=0;i_frame<scene->n_frames;i_frame++)
           scene->perspectives[i_frame]->theta=theta_in;
     }
     // Set p_c values
     for(int i_frame=0;i_frame<scene->n_frames;i_frame++){
        perspective_info *perspective_i=scene->perspectives[i_frame];
        perspective_i->p_c[0]=perspective_i->p_o[0]+perspective_i->radius*cos(perspective_i->zeta)*sin(perspective_i->theta);
        perspective_i->p_c[1]=perspective_i->p_o[1]+perspective_i->radius*cos(perspective_i->zeta)*cos(perspective_i->theta);
        perspective_i->p_c[2]=perspective_i->p_o[2]+perspective_i->radius*sin(perspective_i->zeta);
     }
   }
   else
      SID_trap_error("Invalid quantity {%s} being added to scene in line {%s}.",ERROR_LOGIC,quantity,line);
}

