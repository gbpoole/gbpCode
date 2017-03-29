#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_linear_theory.h>
#include <gbpCosmo_bias.h>
/*
double bias_model_BPR_integral(cosmo_info **cosmo,
                               double       z);
double bias_model_BPR_integral(cosmo_info **cosmo,
                               double       z){
   double       z_max=10000.;
   interp_info *interp;
   if(!ADaPS_exist(*cosmo,"bias_model_BPR_Iz_interp")){
      int     n_int;
      int     i_int;
      double  dz;
      double  Omega_M,Omega_k,Omega_Lambda;
      double  z_temp;
      double *x_int;
      double *y_int;
      double  log_z;
      double  dlog_z;
      n_int       =250;
      Omega_M     =((double *)ADaPS_fetch(*cosmo,"Omega_M"))[0];
      Omega_k     =((double *)ADaPS_fetch(*cosmo,"Omega_k"))[0];
      Omega_Lambda=((double *)ADaPS_fetch(*cosmo,"Omega_Lambda"))[0];
      x_int       = (double *)SID_malloc(sizeof(double)*n_int);
      y_int       = (double *)SID_malloc(sizeof(double)*n_int);
      i_int=0;
      x_int[i_int]=0.;
      y_int[i_int]=pow((1.+x_int[i_int])/E_z(Omega_M,Omega_k,Omega_Lambda,x_int[i_int]),3.);
      i_int++;
      x_int[i_int]=take_log10(z_max)/(double)(n_int-1);
      y_int[i_int]=pow((1.+x_int[i_int])/E_z(Omega_M,Omega_k,Omega_Lambda,x_int[i_int]),3.);
      log_z    =take_log10(x_int[i_int]);
      dlog_z   =(take_log10(z_max)-log_z)/(double)(n_int-2);
      for(i_int++,log_z+=dlog_z;i_int<(n_int-1);i_int++,log_z+=dlog_z){
        x_int[i_int]=take_alog10(log_z);
        y_int[i_int]=pow((1.+x_int[i_int])/E_z(Omega_M,Omega_k,Omega_Lambda,x_int[i_int]),3.);
      }
      x_int[i_int]=z_max;
      y_int[i_int]=pow((1.+x_int[i_int])/E_z(Omega_M,Omega_k,Omega_Lambda,x_int[i_int]),3.);
      init_interpolate(x_int,y_int,(size_t)n_int,gsl_interp_cspline,&interp);
      SID_free(SID_FARG x_int);
      SID_free(SID_FARG y_int);
      ADaPS_store_interp(cosmo,
                         (void *)(interp),
                         "bias_model_BPR_Iz_interp");
          
   }
   else
      interp=(interp_info *)ADaPS_fetch(*cosmo,"bias_model_BPR_Iz_interp");
   return(interpolate_integral(interp,z,z_max));
}
*/

double merger_rate_B_n(cosmo_info **cosmo,
                       int          mode_model,
                       double       z,
                       double       M_0,
                       double       xi){
   double B_n      =-1.;
   int    flag_done=FALSE;

   // Fakhouri & Ma 2008, eqn 10
   if(mode_model==0 || mode_model==1){
      flag_done=TRUE;
      int    mode     =PSPEC_LINEAR_TF;
      int    component=PSPEC_ALL_MATTER;
      double M_prime; 
      // Option 'A' in Fakhouri et al 2008
      if(mode_model==0){
         M_prime=M_0*xi/(1.+xi);
      }
      // Option 'B' in Fakhouri et al 2008
      else if(mode_model==1){
         M_prime=M_0/(1.+xi); 
      }
      double a                  =a_of_z(z);
      //double ddc_dz           =1.260-0.1134*pow(1.+z,-2.)-0.3654*exp(-1.16*z); // From taking the z-derivative of Eqn A7 in Neisenstein and Dekel, 2008
      double ddc_dz             =-1.686*pow(Dplus(a,cosmo),-2.)*dDplus_dz(z,cosmo);
      double sigma_M_0          =sigma_M(cosmo,M_0,    z,mode,component);
      double sigma_M_prime      =sigma_M(cosmo,M_prime,z,mode,component);
      double dlnsigma_dlnM_prime=fabs(dln_sigma_dlnM(cosmo,M_prime,z,mode,component));
      B_n = sqrt(2./PI)*ddc_dz*(1./sigma_M_prime)*dlnsigma_dlnM_prime*pow(1.-((sigma_M_0*sigma_M_0)/(sigma_M_prime*sigma_M_prime)),-1.5);
      // Option 'A' currently not working.  Just return zeros.
      if(mode_model==0)
         B_n=0.;
   }
   // Fakhouri+ 2010
   if(mode_model==2){
      flag_done=TRUE;
      double A      = 0.0104;
      double xi_tild= 9.72e-3;
      double alpha  = 0.133;
      double beta   =-1.995;
      double gamma  = 0.263;
      double eta    = 0.0993;
      B_n = A*pow(M_0/(1e12*M_SOL),alpha)*pow(xi,beta)*exp(pow(xi/xi_tild,gamma))*pow(1+z,eta);
   }
   // Poole+ 2017
   if(mode_model==3){
      flag_done=TRUE;
      double A      = 0.107;
      double xi_tild= 0.475;
      double alpha  = 0.121;
      double beta   =-1.752;
      double gamma  = 0.671;
      double eta    = 0.0643;
      B_n = A*pow(M_0/(1e12*M_SOL),alpha)*pow(xi,beta)*exp(pow(xi/xi_tild,gamma))*pow(1+z,eta);
   }
   // Fakhouri & Ma 2008, eqn 10
   if(mode_model==4){
      flag_done=TRUE;
      double A      = 0.0289;
      double xi_tild= 0.098;
      double alpha  = 0.083;
      double beta   =-2.01;
      double gamma  = 0.409;
      double eta    = 0.371;
      B_n = A*pow(M_0/(1e12*M_SOL),alpha)*pow(xi,beta)*exp(pow(xi/xi_tild,gamma))*pow(1+z,eta);
   }
   if(!flag_done)
      SID_trap_error("Mode flag (%d) is invalid in merger_rate_B_n().  No model definition.",ERROR_LOGIC,mode_model);
   return(B_n);
}

