#define  _MAIN
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo.h>
#include <gbpSPH.h>
#include <gbpHalos.h>
#include <gbpClustering.h>

int main(int argc, char *argv[]){
  int     i,j,i_x,i_y,i_z;
  int     n_2D_total;
  char    species_name[256];
  double  h_Hubble;
  double  Omega_Lambda;
  double  Omega_M;
  double  Omega_b;
  double  f_gas;
  double  Omega_k;
  double  sigma_8;
  double  n_spec;
  double  redshift;
  int     i_rank;
  int     i_grouping;
  int     i_grouping_start;
  int     i_grouping_stop;
  int     n_groupings;
  size_t  n_grouping_local;
  char    filename_in[MAX_FILENAME_LENGTH];
  char    filename_in_root[MAX_FILENAME_LENGTH];
  char    filename_in_model[MAX_FILENAME_LENGTH];
  char    filename_out_1D[MAX_FILENAME_LENGTH];
  char    filename_out_2D[MAX_FILENAME_LENGTH];
  char    filename_out_root[MAX_FILENAME_LENGTH];
  int     i_compute;
  char    grouping_name[6];
  char    filename_TF[256];
  char    n_string[64];
  int             n[3];
  double          x_in,y_in,z_in,vx_in,vy_in,vz_in;
  double          box_size;
  double          L[3];
  size_t          n_all;
  FILE           *fp_in;
  cosmo_info     *cosmo;
  plist_info      plist_header;
  plist_info      plist;
  FILE           *fp_1D;
  FILE           *fp_2D;
  int     flag_write_header=TRUE;
  size_t  n_grouping;
  int   n_halos,i_halo,j_halo;
  int   n_particles_min;
  GBPREAL *x_halos;
  GBPREAL *y_halos;
  GBPREAL *z_halos;
  GBPREAL *vx_halos_sub;
  GBPREAL *vy_halos_sub;
  GBPREAL *vz_halos_sub;
  GBPREAL *vx_halos_FoF;
  GBPREAL *vy_halos_FoF;
  GBPREAL *vz_halos_FoF;
  GBPREAL *vx_halos_sys;
  GBPREAL *vy_halos_sys;
  GBPREAL *vz_halos_sys;
  GBPREAL *vx_halos;
  GBPREAL *vy_halos;
  GBPREAL *vz_halos;
  GBPREAL *V_halos;
  double *M_halos;
  size_t *M_halos_index;
  GBPREAL *x_grouping;
  GBPREAL *y_grouping;
  GBPREAL *z_grouping;
  GBPREAL *vx_grouping;
  GBPREAL *vy_grouping;
  GBPREAL *vz_grouping;
  GBPREAL  x_min,x_max;
  GBPREAL  y_min,y_max;
  GBPREAL  z_min,z_max;
  int     n_particles;
  double  M_min,M_max,M_med;
  double  V_min,V_max,V_med;
  char   *line=NULL;
  size_t  line_length=0;
  int     i_bin,j_bin;
  int    i_file,n_files;
  int    mass_assignment_scheme;
  int     n_1D,n_2D;
  RNG_info  RNG;
  int       seed=1327621;
  long long   n_data_ll;
  int        *rank_own;
  int        *rank_own_index;
  size_t      n_rank;
  int         flag_init_store;
  
  int        grid_size; 
  double     k_min_1D;
  double     k_max_1D; 
  double     dk_1D; 
  double     k_min_2D; 
  double     k_max_2D; 
  double     dk_2D; 
  field_info   FFT; 
  int        n_k_1D; 
  double    *k_1D; 
  double    *P_k_1D; 
  double    *dP_k_1D; 
  int       *n_modes_1D; 
  int        n_k_2D; 
  double    *P_k_2D; 
  double    *dP_k_2D; 
  int       *n_modes_2D;
  int        pspec_mode;

  // Initialization -- MPI etc.
  SID_init(&argc,&argv,NULL);
  if(argc!=8)
    SID_trap_error("Incorrect syntax.",ERROR_SYNTAX);
  strcpy(filename_in_root, argv[1]);
  strcpy(filename_out_root,argv[2]);
  redshift         =(double)atof(argv[3]);
  grid_size        =(int)   atoi(argv[4]);
  box_size         =(double)atof(argv[5]);
  i_grouping_start =(int)   atoi(argv[6]);
  i_grouping_stop  =(int)   atoi(argv[7]);
  n_groupings      =i_grouping_stop-i_grouping_start+1;
  if(n_groupings<1)
      SID_trap_error("No groupings have been selected (you chose start=%d, stop=%d).",ERROR_LOGIC,i_grouping_start,i_grouping_stop);

  sprintf(filename_out_1D,   "%s.1D_pspec",   filename_out_root);
  sprintf(filename_out_2D,   "%s.2D_pspec",   filename_out_root);

  SID_log("Producing P(k)'s...",SID_LOG_OPEN);

  // Set the k ranges
  k_min_1D=0.0;
  k_max_1D=0.3;
  dk_1D   =0.01;
  k_min_2D=0.0;
  k_max_2D=0.3;
  dk_2D   =0.02;

  // Set mass assignment scheme
  mass_assignment_scheme=MAP2GRID_DIST_DWT20;

  // Initialization
  n[0]=grid_size;
  n[1]=n[0];
  n[2]=n[0];
  L[0]=box_size;
  L[1]=L[0];
  L[2]=L[0];
  init_plist(&plist,NULL,GADGET_LENGTH,GADGET_MASS,GADGET_VELOCITY);
  init_cosmo_std(&cosmo);
  init_field(3,n,L,&FFT);
  init_RNG(&seed,&RNG,RNG_DEFAULT); // Needed for rank assignment

  // Initialize arrays
  SID_log("Initializing arrays...",SID_LOG_OPEN);
  (n_k_1D)    =(int)(0.5+(k_max_1D-k_min_1D)/dk_1D);
  (k_1D)      =(double *)SID_malloc(sizeof(double)*(n_k_1D)); 
  (P_k_1D)    =(double *)SID_malloc(sizeof(double)*(n_k_1D)); 
  (dP_k_1D)   =(double *)SID_malloc(sizeof(double)*(n_k_1D)); 
  (n_modes_1D)=(int    *)SID_malloc(sizeof(int)*(n_k_1D)); 
  (n_k_2D)    =(int)(0.5+(k_max_2D-k_min_2D)/dk_1D);
  (P_k_2D)    =(double *)SID_malloc(sizeof(double)*(n_k_2D)*(n_k_2D));
  (dP_k_2D)   =(double *)SID_malloc(sizeof(double)*(n_k_2D)*(n_k_2D));
  (n_modes_2D)=(int    *)SID_malloc(sizeof(int)*(n_k_2D)*(n_k_2D));
  SID_log("Done.",SID_LOG_CLOSE);

  // Initialize files
  if(SID.I_am_Master){
    fp_1D   =fopen(filename_out_1D,   "w");
    fp_2D   =fopen(filename_out_2D,   "w");
  }

  // Process each grouping in turn
  for(i_grouping=i_grouping_start,flag_init_store=TRUE;i_grouping<=i_grouping_stop;i_grouping++){
    SID_log("Processing grouping #%03d...",SID_LOG_OPEN|SID_LOG_TIMER,i_grouping);

    // Read catalog
    SID_log("Reading catalog {%s}...",SID_LOG_OPEN,filename_in);
    if(SID.I_am_Master){
      fp_in=fopen(filename_in,"r");
      n_grouping=(size_t)count_lines_data(fp_in);
      for(i_halo=0,n_grouping_local=0;i_halo<n_grouping;i_halo++){
         grab_next_line_data(fp_in,&line,&line_length);
         grab_double(line,7, &x_in);
         if(x_in>=FFT.slab.x_min_local && x_in<FFT.slab.x_max_local)
            n_grouping_local++;
      }
      rewind(fp_in);
      x_grouping  =(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      y_grouping  =(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      z_grouping  =(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vx_halos_sub=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vy_halos_sub=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vz_halos_sub=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vx_halos_FoF=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vy_halos_FoF=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vz_halos_FoF=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vx_halos_sys=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vy_halos_sys=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      vz_halos_sys=(GBPREAL  *)SID_malloc(sizeof(GBPREAL)*n_grouping_local);
      rank_own    =(int   *)SID_malloc(sizeof(int) *n_grouping_local);
      x_min=1e10;
      x_max=0.;
      y_min=1e10;
      y_max=0.;
      z_min=1e10;
      z_max=0.;
      for(i_halo=0,n_grouping_local=0;i_halo<n_grouping;i_halo++){
        grab_next_line_data(fp_in,&line,&line_length);
        grab_double(line,7, &x_in);
        if(x_in>=FFT.slab.x_min_local && x_in<FFT.slab.x_max_local){
           grab_double(line,8, &y_in);
           grab_double(line,9, &z_in);
           grab_double(line,11,&vx_in);
           grab_double(line,12,&vy_in);
           grab_double(line,13,&vz_in);
           x_grouping[n_grouping_local] =(GBPREAL)x_in;
           y_grouping[n_grouping_local] =(GBPREAL)y_in;
           z_grouping[n_grouping_local] =(GBPREAL)z_in;
           x_min=MIN(x_min,x_halos[i_halo]);
           x_max=MAX(x_max,x_halos[i_halo]);
           y_min=MIN(y_min,y_halos[i_halo]);
           y_max=MAX(y_max,y_halos[i_halo]);
           z_min=MIN(z_min,z_halos[i_halo]);
           z_max=MAX(z_max,z_halos[i_halo]);
           vx_halos_sub[i_halo]=(GBPREAL)vx_in;
           vy_halos_sub[i_halo]=(GBPREAL)vy_in;
           vz_halos_sub[i_halo]=(GBPREAL)vz_in;
           grab_double(line,14,&vx_in);
           grab_double(line,15,&vy_in);
           grab_double(line,16,&vz_in);
           vx_halos_FoF[i_halo]=(GBPREAL)vx_in;
           vy_halos_FoF[i_halo]=(GBPREAL)vy_in;
           vz_halos_FoF[i_halo]=(GBPREAL)vz_in;
           grab_double(line,17,&vx_in);
           grab_double(line,18,&vy_in);
           grab_double(line,19,&vz_in);
           vx_halos_sys[i_halo]=(GBPREAL)vx_in;
           vy_halos_sys[i_halo]=(GBPREAL)vy_in;
           vz_halos_sys[i_halo]=(GBPREAL)vz_in;           
           n_grouping_local++;
        }
      }
      SID_log("x_range=%le->%le",SID_LOG_COMMENT,x_min,x_max);
      SID_log("y_range=%le->%le",SID_LOG_COMMENT,y_min,y_max);
      SID_log("z_range=%le->%le",SID_LOG_COMMENT,z_min,z_max);
      fclose(fp_in);
    }
    SID_Bcast(&n_grouping,sizeof(size_t),MASTER_RANK,SID.COMM_WORLD);

    // Loop over 3 velocity sets
    vx_grouping=vx_halos_sub;
    vy_grouping=vy_halos_sub;
    vz_grouping=vz_halos_sub;

    // Store grouping
    ADaPS_store(&(plist.data),(void *)&n_grouping,      "n_all_halos", ADaPS_SCALAR_SIZE_T);
    ADaPS_store(&(plist.data),(void *)&n_grouping_local,"n_halos",     ADaPS_SCALAR_SIZE_T);
    ADaPS_store(&(plist.data),(void *)x_grouping,       "x_halos",     ADaPS_DEFAULT);
    ADaPS_store(&(plist.data),(void *)y_grouping,       "y_halos",     ADaPS_DEFAULT);
    ADaPS_store(&(plist.data),(void *)z_grouping,       "z_halos",     ADaPS_DEFAULT);
    ADaPS_store(&(plist.data),(void *)vx_grouping,      "vx_halos",    ADaPS_DEFAULT);
    ADaPS_store(&(plist.data),(void *)vy_grouping,      "vy_halos",    ADaPS_DEFAULT);
    ADaPS_store(&(plist.data),(void *)vz_grouping,      "vz_halos",    ADaPS_DEFAULT);
    SID_log("Done.",SID_LOG_CLOSE);

    // Needed to make writing more straight-forward
    n_data_ll  =(long long)n_grouping;
    
    for(i_compute=0;i_compute<4;i_compute++){
      switch(i_compute){
      case 0:
        SID_log("Processing real-space ...",SID_LOG_OPEN|SID_LOG_TIMER);
        pspec_mode=PSPEC_DEFAULT;
        break;
      case 1:
        SID_log("Processing v_x redshift space...",SID_LOG_OPEN|SID_LOG_TIMER);
        pspec_mode=PSPEC_ADD_VX;
        break;
      case 2:
        SID_log("Processing v_y redshift space...",SID_LOG_OPEN|SID_LOG_TIMER);
        pspec_mode=PSPEC_ADD_VY;
        break;
      case 3:
        SID_log("Processing v_z redsift space...",SID_LOG_OPEN|SID_LOG_TIMER);
        pspec_mode=PSPEC_ADD_VZ;
        break;
      }

      // Compute power spectrum
      compute_power_spectrum(&plist,
                             &FFT,
                             mass_assignment_scheme,
                             pspec_mode,
                             cosmo,
                             "halos",
                             redshift,
                             n_k_1D,
                             k_min_1D,
                             k_max_1D,
                             n_k_2D,
                             k_min_2D,
                             k_max_2D,
                             k_1D,
                             P_k_1D,
                             dP_k_1D,
                             n_modes_1D,
                             P_k_2D,
                             dP_k_2D,
                             n_modes_2D);

      // Write power spectra
      if(SID.I_am_Master){
        // Write 1D power spectra
        if(flag_write_header){
          fwrite(&n_groupings,sizeof(int),   1,     fp_1D);
          fwrite(&n_k_1D,     sizeof(int),   1,     fp_1D);
          fwrite(&k_min_1D,   sizeof(double),1,     fp_1D);
          fwrite(&k_max_1D,   sizeof(double),1,     fp_1D);
          fwrite(&dk_1D,      sizeof(double),1,     fp_1D);
          fwrite(k_1D,        sizeof(double),n_k_1D,fp_1D);
        }
        fwrite(P_k_1D,    sizeof(double),n_k_1D,fp_1D);
        fwrite(dP_k_1D,   sizeof(double),n_k_1D,fp_1D);
        fwrite(n_modes_1D,sizeof(int),   n_k_1D,fp_1D);

        // Write 2D power spectra
        if(flag_write_header){
          fwrite(&n_groupings,sizeof(int),   1,fp_2D);
          fwrite(&n_k_2D,     sizeof(int),   1,fp_2D);
          fwrite(&k_min_2D,   sizeof(double),1,fp_2D);
          fwrite(&k_max_2D,   sizeof(double),1,fp_2D);
          fwrite(&dk_2D,      sizeof(double),1,fp_2D);
        }
        fwrite(P_k_2D,    sizeof(double),n_k_2D*n_k_2D,fp_2D);
        fwrite(dP_k_2D,   sizeof(double),n_k_2D*n_k_2D,fp_2D);
        fwrite(n_modes_2D,sizeof(int),   n_k_2D*n_k_2D,fp_2D);

        flag_write_header=FALSE;
      }
      
      SID_log("Done.",SID_LOG_CLOSE);
    } // Loop over 4 P(k)'s

    // Loop over 3 velocity sets

    // Clean-up
    SID_log("Cleaning-up...",SID_LOG_OPEN);
    if(SID.I_am_Master){
      SID_free(SID_FARG rank_own);
      SID_free(SID_FARG x_grouping);
      SID_free(SID_FARG y_grouping);
      SID_free(SID_FARG z_grouping);
      SID_free(SID_FARG vx_halos_sub);
      SID_free(SID_FARG vy_halos_sub);
      SID_free(SID_FARG vz_halos_sub);
      SID_free(SID_FARG vx_halos_FoF);
      SID_free(SID_FARG vy_halos_FoF);
      SID_free(SID_FARG vz_halos_FoF);
      SID_free(SID_FARG vx_halos_sys);
      SID_free(SID_FARG vy_halos_sys);
      SID_free(SID_FARG vz_halos_sys);
    }

    SID_log("Done.",SID_LOG_CLOSE);

    SID_log("Done.",SID_LOG_CLOSE);
  } // Loop over groupings

  // Close files
  if(SID.I_am_Master){
    fclose(fp_1D);
    fclose(fp_2D);
  }
  
  // Clean-up
  SID_log("Cleaning-up...",SID_LOG_OPEN);
  free_plist(&plist);
  free_RNG(&RNG);
  SID_free(SID_FARG k_1D);
  SID_free(SID_FARG P_k_1D); 
  SID_free(SID_FARG dP_k_1D);
  SID_free(SID_FARG n_modes_1D);
  SID_free(SID_FARG P_k_2D);
  SID_free(SID_FARG dP_k_2D);
  SID_free(SID_FARG n_modes_2D);
  SID_log("Done.",SID_LOG_CLOSE);

  SID_log("Done.",SID_LOG_CLOSE);
  SID_exit(ERROR_NONE);
}
