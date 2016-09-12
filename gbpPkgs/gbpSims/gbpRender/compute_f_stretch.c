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
         if(z_i>0.)
            f_i=(float)d_image_plane/(float)z_i;
         else
            f_i=0.;
         break;
      case TRUE:
         f_i=1.;
         break;
   }
   return(f_i);
}

