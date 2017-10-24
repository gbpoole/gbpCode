#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void set_pixel_space(float   h_i,
                     float   x_i,
                     float   y_i,
                     float   f_i,
                     double  xmin,
                     double  ymin,
                     double  FOV_x,
                     double  FOV_y,
                     double  pixel_size_x,
                     double  pixel_size_y,
                     double  radius_kernel_max,
                     double *radius2_norm,
                     double *radius_kernel,
                     double *part_pos_x,
                     double *part_pos_y,
                     int    *kx_min,
                     int    *kx_max,
                     int    *ky_min,
                     int    *ky_max){
   double part_h_xy;
   part_h_xy       =(double)h_i*(double)f_i;
   (*part_pos_x)   =(double)x_i*(double)f_i;
   (*part_pos_y)   =(double)y_i*(double)f_i;
   (*radius2_norm) =1./(part_h_xy*part_h_xy);
   (*radius_kernel)=radius_kernel_max*part_h_xy;
   (*kx_min)       =(int)(((*part_pos_x)-(*radius_kernel)-xmin)/pixel_size_x);
   (*kx_max)       =(int)(((*part_pos_x)+(*radius_kernel)-xmin)/pixel_size_x+ONE_HALF);
   (*ky_min)       =(int)(((*part_pos_y)-(*radius_kernel)-ymin)/pixel_size_y);
   (*ky_max)       =(int)(((*part_pos_y)+(*radius_kernel)-ymin)/pixel_size_y+ONE_HALF);
}
