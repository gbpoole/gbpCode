#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

float compute_f_stretch(double d_image_plane,float z_i,int flag_plane_parallel){
   float f_i;
   switch(flag_plane_parallel){
      case FALSE:
         if(z_i>0.f) 
            f_i=(float)d_image_plane/(float)z_i;
         else // save the divisions if we won't be rendering the particle anyways
            f_i=0.f;
         break;
      case TRUE:
         f_i=1.f;
         break;
   }
   return(f_i);
}

