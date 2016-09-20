#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

// Compute the angle and the axis of rotation
//   needed to place the camera at (0,0,-d_o)
//   with the object at (0,0,0)
void compute_perspective_transformation(double  x_o,
                                        double  y_o,
                                        double  z_o,
                                        double  x_c,
                                        double  y_c,
                                        double  z_c,
                                        double  unit_factor,
                                        const char *unit_text,
                                        double  f_image_plane,
                                        double  stereo_offset,
                                        double *FOV_x,
                                        double *FOV_y,
                                        double *d_o,
                                        double *x_o_out,
                                        double *y_o_out,
                                        double *z_o_out,
                                        double *x_c_out,
                                        double *y_c_out,
                                        double *z_c_out,
                                        double *x_hat,
                                        double *y_hat,
                                        double *z_hat,
                                        double *theta,
                                        double *theta_roll){
  double d_x_o;
  double d_y_o;
  double d_z_o;
  double d_xy;
  d_x_o   = x_o-x_c;
  d_y_o   = y_o-y_c;
  d_z_o   = z_o-z_c;
  d_xy    = sqrt(pow(d_x_o,2.)+pow(d_y_o,2.));
  (*d_o)  = sqrt(pow(d_x_o,2.)+pow(d_y_o,2.)+pow(d_z_o,2.));
  (*x_hat)= d_y_o/d_xy;
  (*y_hat)=-d_x_o/d_xy;
  (*z_hat)= 0.;
  (*theta)= acos(d_z_o/(*d_o));
  if(sqrt(d_x_o*d_x_o+d_y_o*d_y_o)>0.){
    (*theta_roll)=acos(-d_y_o/sqrt(d_x_o*d_x_o+d_y_o*d_y_o));
    if(d_x_o<0.)
      (*theta_roll)=TWO_PI-(*theta_roll);
  }
  else
    (*theta_roll)=0.;

  // Apply stereo offsets
  (*x_o_out)=x_o;
  (*y_o_out)=y_o;
  (*z_o_out)=z_o;
  (*x_c_out)=x_c;
  (*y_c_out)=y_c;
  (*z_c_out)=z_c;
  if(stereo_offset!=0.){
     double Dx_stereo;
     double Dy_stereo;
     double Dz_stereo;
     double theta_roll_stereo=0.;
     double cos_theta_over_d_xy;
     SID_log("Forcing theta=0 in stereo projection.",SID_LOG_COMMENT);
     if(d_x_o>0.){
        if(d_y_o>0.){
           Dx_stereo=-stereo_offset*cos(theta_roll_stereo)*fabs(d_y_o)/d_xy;
           Dy_stereo= stereo_offset*cos(theta_roll_stereo)*fabs(d_x_o)/d_xy;
        }
        else{
           Dx_stereo= stereo_offset*cos(theta_roll_stereo)*fabs(d_y_o)/d_xy;
           Dy_stereo= stereo_offset*cos(theta_roll_stereo)*fabs(d_x_o)/d_xy;
        }
     }
     else{
        if(d_y_o>0.){
           Dx_stereo=-stereo_offset*cos(theta_roll_stereo)*fabs(d_y_o)/d_xy;
           Dy_stereo=-stereo_offset*cos(theta_roll_stereo)*fabs(d_x_o)/d_xy;
        }
        else{
           Dx_stereo= stereo_offset*cos(theta_roll_stereo)*fabs(d_y_o)/d_xy;
           Dy_stereo=-stereo_offset*cos(theta_roll_stereo)*fabs(d_x_o)/d_xy;
        }
     }
     if(theta_roll_stereo>0.)
        Dz_stereo=sqrt(stereo_offset*stereo_offset-Dx_stereo*Dx_stereo-Dy_stereo*Dy_stereo);
     else if(theta_roll_stereo==0.)
        Dz_stereo=0.;
     else
        Dz_stereo=-sqrt(stereo_offset*stereo_offset-Dx_stereo*Dx_stereo-Dy_stereo*Dy_stereo);
     //if(stereo_offset!=0){
     //   SID_log("Stereo offset results: (offset=%10.3le [%s])",SID_LOG_OPEN,   stereo_offset/unit_factor,unit_text);
     //   SID_log("(x_o,x_c,D_x)=(%10.3le,%10.3le,%10.3le) [%s]",SID_LOG_COMMENT,x_o/unit_factor,x_c/unit_factor,Dx_stereo/unit_factor,unit_text);
     //   SID_log("(y_o,y_c,D_y)=(%10.3le,%10.3le,%10.3le) [%s]",SID_LOG_COMMENT,y_o/unit_factor,y_c/unit_factor,Dy_stereo/unit_factor,unit_text);
     //   SID_log("(z_o,z_c,D_z)=(%10.3le,%10.3le,%10.3le) [%s]",SID_LOG_COMMENT,z_o/unit_factor,z_c/unit_factor,Dz_stereo/unit_factor,unit_text);
     //   SID_log("",SID_LOG_SILENT_CLOSE);
     //}
     (*x_o_out)+=Dx_stereo;
     (*y_o_out)+=Dy_stereo;
     (*z_o_out)+=Dz_stereo;
     (*x_c_out)+=Dx_stereo;
     (*y_c_out)+=Dy_stereo;
     (*z_c_out)+=Dz_stereo;
  }

  // Recompute d_o to place the object in front/behind of the image plane
  (*d_o)  /=f_image_plane;
  (*FOV_x)*=f_image_plane;
  (*FOV_y)*=f_image_plane;

}
