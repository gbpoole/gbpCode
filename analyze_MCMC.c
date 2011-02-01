#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpRNG.h>
#include <gbpMCMC.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_interp.h>

void analyze_MCMC(MCMC_info *MCMC){
  char      filename_output_dir[MAX_FILENAME_LENGTH];
  char      filename_chain_dir[MAX_FILENAME_LENGTH];
  char      filename_results_dir[MAX_FILENAME_LENGTH];
  char      filename_plots_dir[MAX_FILENAME_LENGTH];
  char      filename_run[MAX_FILENAME_LENGTH];
  char      filename_chain[MAX_FILENAME_LENGTH];
  char      filename_chain_iterations[MAX_FILENAME_LENGTH];
  char      filename_stats[MAX_FILENAME_LENGTH];
  char      filename_coverage[MAX_FILENAME_LENGTH];
  char      filename_chain_covariance[MAX_FILENAME_LENGTH];
  char      filename_covariance[MAX_FILENAME_LENGTH];
  char      filename_histograms[MAX_FILENAME_LENGTH];
  char      filename_results[MAX_FILENAME_LENGTH];
  char      filename_stop[MAX_FILENAME_LENGTH];
  char      column_txt[MAX_FILENAME_LENGTH];
  char      problem_name_test[MCMC_NAME_SIZE];
  char      P_name_test[MCMC_NAME_SIZE];
  char      name_test[MCMC_NAME_SIZE];
  char      array_name_test[MCMC_NAME_SIZE];
  char      array_name_temp[MCMC_NAME_SIZE];
  int       n_avg_test;
  int       flag_autocor_on_test;
  int       n_P_test;
  int       n_DS_test;
  int       n_arrays_test;
  int       n_M_test;
  double    P_init_test;
  double    M_target_test;
  double    dM_target_test;
  int       i_P,j_P,k_P;
  int       i_M,j_M;
  int       ii_P,jj_P;
  int       i_avg;
  int       i_C;
  int       i_DS,n_DS;
  int       i_iteration,j_iteration;
  int       flag_continue;
  char      flag_success;
  int       flag_stop=FALSE;
  int       i_coverage;
  int       j_coverage;
  int       i_proposal;
  int       bin_x;
  int       bin_y;
  int       n_avg;
  int       n_avg_burn;
  int       n_avg_integrate;
  int       n_P,n_C;
  int       n_gals;
  size_t    n_success,n_fail;
  size_t    n_success_all,n_fail_all;
  double   *x_P;
  double  **x_M;
  double   *M_target;
  double   *dM_target;
  double   *V;
  double  **C;
  double   *P_min;
  double   *P_max;
  double   *P_new;
  double   *P_last;
  double   *P_avg;
  double   *dP_avg;
  double    ln_Pr_last;
  double    ln_Pr_new;
  double    ln_Pr_min;
  double    ln_Pr_avg;
  double    ln_Pr_max;
  double  **M_best;
  double  **M_min;
  double  **M_max;
  double  **M_new;
  double  **M_lo_68;
  double  **M_hi_68;
  double  **M_lo_95;
  double  **M_hi_95;
  size_t ***M_histogram;
  double  **M_last;
  double  **M_avg;
  double  **dM_avg;
  size_t **P_histogram;
  size_t **coverage_true;
  size_t **coverage_false;
  size_t **coverage_keep;
  double    L_x,L_y,L_z;
  int       n_x,n_y,n_z;
  int       n_C_x;
  double    ln_likelihood_new,ln_likelihood_last;
  FILE     *fp_run;
  FILE     *fp_chain;
  FILE     *fp_chain_iterations;
  FILE     *fp_stats;
  FILE     *fp_coverage;
  FILE     *fp_chain_covariance;
  FILE     *fp_covariance;
  FILE     *fp_histograms;
  FILE     *fp_results;
  FILE     *fp_stop;
  RNG_info  RNG;
  int       seed=182743;
  int       i_report;
  int       i_iteration_start;
  int       i_iteration_next_report;
  int       n_accepted;
  int       n_active;
  int       n_iterations;
  int       n_iterations_burn;
  int       n_iterations_integrate;
  int       n_iterations_phase;
  int       n_thin;
  int       n_thin_burn;
  int       n_thin_integrate;
  int       i_thin;
  double    temperature;
  int       flag_autocor_on;
  int       n_coverage;
  int       coverage_size;
  int       i_phase;
  int       i_array;
  int      *n_M;
  int       flag_initialized;
  int       flag_report_props;
  int       n_used;
  gsl_matrix *m;
  gsl_vector *b;
  double      RN;
  MCMC_DS_info  *current_DS;
  MCMC_DS_info  *next_DS;
  double       ***M_arrays;
  double       ***P_arrays;
  int            *n_P_arrays;
  int            *n_M_arrays;
  double          constant;
  size_t         *histogram_index;
  size_t         *coverage_keep_index;
  size_t          P_best_index;
  size_t          M_best_index;
  double          accumulator;
  size_t          P_lo_68_index;
  size_t          P_hi_68_index;
  size_t          P_lo_95_index;
  size_t          P_hi_95_index;
  size_t          M_lo_68_index;
  size_t          M_hi_68_index;
  size_t          M_lo_95_index;
  size_t          M_hi_95_index;
  double         *P_lo_68;
  double         *P_hi_68;
  double         *P_lo_95;
  double         *P_hi_95;
  double         *P_contour_68;
  double         *P_contour_95;
  double         *P_best;
  int             my_chain;
  int             flag_init;
  int             i_column;
  int             dummy_t;
  double         *P_i_bar_accum;
  double         *P_ij_bar_accum;
  double         *P_i_bar;
  double         *P_ij_bar;
  double         *V_compute;
  int             n_covariance;
  int             i_covariance;
  int             j_covariance;
  int             n_iterations_file_total;
  int             n_iterations_file_burn;
  int             flag_restart=FALSE;

  SID_log("Performing MCMC analysis...",SID_LOG_OPEN|SID_LOG_TIMER);

  SID_log("Initializing...",SID_LOG_OPEN);

  // Set the local chain number
  if(check_mode_for_flag(MCMC->mode,MCMC_MODE_PARALLEL))
    my_chain=SID.My_rank;
  else
    my_chain=MASTER_RANK;

  // Initialize some constants etc.
  n_P                   =MCMC->n_P;
  n_DS                  =MCMC->n_DS;
  n_avg                 =MCMC->n_avg;
  n_iterations          =MCMC->n_iterations;
  n_iterations_burn     =MCMC->n_iterations_burn;
  n_iterations_integrate=n_iterations-n_iterations_burn;
  n_thin                =MCMC->n_thin;
  coverage_size         =MCMC->coverage_size;
  flag_autocor_on       =MCMC->flag_autocor_on;
  flag_report_props     =check_mode_for_flag(MCMC->mode,MCMC_REPORT_PROPS);  
  strcpy(filename_output_dir,MCMC->filename_output_dir);

  // Initialize arrays used for computing the covariance matrix
  P_i_bar_accum =(double *)SID_malloc(sizeof(double)*n_P);
  P_ij_bar_accum=(double *)SID_malloc(sizeof(double)*n_P*n_P);
  P_i_bar       =(double *)SID_malloc(sizeof(double)*n_P);
  P_ij_bar      =(double *)SID_malloc(sizeof(double)*n_P*n_P);
  V_compute     =(double *)SID_malloc(sizeof(double)*n_P*n_P);
  for(i_P=0,k_P=0;i_P<n_P;i_P++){
    P_i_bar_accum[i_P]=0.;
    P_i_bar[i_P]      =0.;
    for(j_P=0;j_P<n_P;j_P++,k_P++){
      P_ij_bar_accum[k_P]=0.;
      P_ij_bar[k_P]      =0.;
      if(i_P==j_P)
        V_compute[k_P]=1.;
      else
        V_compute[k_P]=0.;
    }
  }

  // Initialize chain arrays etc
  n_M       =(int      *)SID_malloc(sizeof(int      )*n_DS);
  M_new     =(double  **)SID_malloc(sizeof(double  *)*n_DS);
  M_last    =(double  **)SID_malloc(sizeof(double  *)*n_DS);
  n_M_arrays=(int      *)SID_malloc(sizeof(int      )*n_DS);
  M_arrays  =(double ***)SID_malloc(sizeof(double **)*n_DS);
  current_DS=MCMC->DS;
  i_DS      =0;
  while(current_DS!=NULL){
    next_DS         =current_DS->next;
    n_M[i_DS]       =current_DS->n_M;
    M_new[i_DS]     =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
    M_last[i_DS]    =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
    n_M_arrays[i_DS]=current_DS->n_arrays;
    M_arrays[i_DS]  =current_DS->array;
    current_DS      =next_DS;
    i_DS++;
  }
  P_new          =(double  *)SID_malloc(sizeof(double)*n_P);
  P_last         =(double  *)SID_malloc(sizeof(double)*n_P);
  P_min          =(double  *)SID_malloc(sizeof(double)*n_P);
  P_max          =(double  *)SID_malloc(sizeof(double)*n_P);
  P_avg          =(double  *)SID_malloc(sizeof(double)*n_P);
  dP_avg         =(double  *)SID_malloc(sizeof(double)*n_P);
  for(i_P=0;i_P<n_P;i_P++){
    P_min[i_P] =DBL_MAX;
    P_max[i_P] =DBL_MIN;
    P_avg[i_P] =0.;
    dP_avg[i_P]=0.;
  }

  // Set directories
  sprintf(filename_output_dir, "%s/",        MCMC->filename_output_dir);
  sprintf(filename_chain_dir,  "%s/chains/", filename_output_dir);
  sprintf(filename_results_dir,"%s/results/",filename_output_dir);
  sprintf(filename_plots_dir,  "%s/plots/",  filename_output_dir);

  // Set filenames
  sprintf(filename_run,             "%s/run.dat",                  filename_output_dir);
  sprintf(filename_chain,           "%s/chain_trace_%06d.dat",     filename_chain_dir,my_chain);
  sprintf(filename_chain_iterations,"%s/chain_iterations_%06d.dat",filename_chain_dir,my_chain);
  sprintf(filename_chain_covariance,"%s/chain_covariance_%06d.dat",filename_chain_dir,my_chain);
  sprintf(filename_stats,           "%s/chain_stats_%06d.dat",     filename_chain_dir,my_chain);
  sprintf(filename_coverage,        "%s/coverage.dat",             filename_results_dir);
  sprintf(filename_histograms,      "%s/histograms.dat",           filename_results_dir);
  sprintf(filename_covariance,      "%s/covariance.dat",           filename_results_dir);
  sprintf(filename_stop,            "%s/stop",                     filename_output_dir);
  
    // Initialize coverage arrays
    for(i_P=0,n_coverage=0;i_P<n_P;i_P++){
        for(j_P=i_P+1;j_P<n_P;j_P++)
            n_coverage++;
    }
    P_contour_68  =(double   *)SID_malloc(sizeof(double)*n_coverage);
    P_contour_95  =(double   *)SID_malloc(sizeof(double)*n_coverage);
    P_best        =(double   *)SID_malloc(sizeof(double)*n_P);
    P_lo_68       =(double   *)SID_malloc(sizeof(double)*n_P);
    P_hi_68       =(double   *)SID_malloc(sizeof(double)*n_P);
    P_lo_95       =(double   *)SID_malloc(sizeof(double)*n_P);
    P_hi_95       =(double   *)SID_malloc(sizeof(double)*n_P);
    P_histogram   =(size_t  **)SID_malloc(sizeof(size_t *)*n_P);
    for(i_P=0;i_P<n_P;i_P++){
      P_histogram[i_P]=(size_t *)SID_malloc(sizeof(size_t)*coverage_size);
      P_min[i_P]      =DBL_MAX;
      P_max[i_P]      =DBL_MIN;
      for(j_P=0;j_P<coverage_size;j_P++)
        P_histogram[i_P][j_P]=0;
    }
    M_best        =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_min         =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_max         =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_lo_68       =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_hi_68       =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_lo_95       =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_hi_95       =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_avg         =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    dM_avg        =(double  **)SID_malloc(sizeof(double  *)*n_DS);
    M_histogram   =(size_t ***)SID_malloc(sizeof(size_t **)*n_DS);
    for(i_DS=0;i_DS<n_DS;i_DS++){
      M_best[i_DS]     =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_min[i_DS]      =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_max[i_DS]      =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_lo_68[i_DS]    =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_hi_68[i_DS]    =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_lo_95[i_DS]    =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_hi_95[i_DS]    =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_avg[i_DS]      =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      dM_avg[i_DS]     =(double *)SID_malloc(sizeof(double)*n_M[i_DS]);
      M_histogram[i_DS]=(size_t **)SID_malloc(sizeof(size_t *)*n_M[i_DS]);
      for(i_M=0;i_M<n_M[i_DS];i_M++){
        M_histogram[i_DS][i_M]=(size_t *)SID_malloc(sizeof(size_t)*coverage_size);
        M_avg[i_DS][i_M]  =0.;
        dM_avg[i_DS][i_M] =0.;
        M_min[i_DS][i_M]  =DBL_MAX;
        M_max[i_DS][i_M]  =DBL_MIN;
        for(j_M=0;j_M<coverage_size;j_M++)
          M_histogram[i_DS][i_M][j_M]=0;
      }
    }
    coverage_true =(size_t  **)SID_malloc(sizeof(size_t  *)*n_coverage);
    coverage_false=(size_t  **)SID_malloc(sizeof(size_t  *)*n_coverage);
    coverage_keep =(size_t  **)SID_malloc(sizeof(size_t  *)*n_coverage);
    for(i_coverage=0;i_coverage<n_coverage;i_coverage++){
      coverage_true[i_coverage] =(size_t *)SID_malloc(sizeof(size_t)*coverage_size*coverage_size);
      coverage_false[i_coverage]=(size_t *)SID_malloc(sizeof(size_t)*coverage_size*coverage_size);
      coverage_keep[i_coverage] =(size_t *)SID_malloc(sizeof(size_t)*coverage_size*coverage_size);
      for(j_coverage=0;j_coverage<coverage_size*coverage_size;j_coverage++){
        coverage_true[i_coverage][j_coverage] =0;
        coverage_false[i_coverage][j_coverage]=0;
        coverage_keep[i_coverage][j_coverage] =0;
      }
    }
    SID_log("Done.",SID_LOG_CLOSE);

    // Process the chain(s)
    SID_log("Process chain file(s)...",SID_LOG_OPEN);
    if(my_chain==SID.My_rank){
      fp_chain=fopen(filename_chain,"rb");
      for(i_iteration=0,flag_init=TRUE,n_used=0;i_iteration<n_iterations;i_iteration++){
        for(i_avg=0;i_avg<n_avg;i_avg++){
          fread(&flag_success,sizeof(char),  1,  fp_chain);
          fread(&ln_Pr_last,  sizeof(double),1,  fp_chain);
          fread(P_new,        sizeof(double),n_P,fp_chain);
          for(i_DS=0;i_DS<n_DS;i_DS++)
            fread(M_new[i_DS],sizeof(double),n_M[i_DS],fp_chain);
          if(i_iteration>=n_iterations_burn){
            if(flag_init || flag_success){
              memcpy(P_last,P_new,(size_t)n_P*sizeof(double));      
              for(i_DS=0;i_DS<n_DS;i_DS++)
                memcpy(M_last[i_DS],M_new[i_DS],(size_t)n_M[i_DS]*sizeof(double));
              flag_init=FALSE;
            }
            n_used++;

            // Compute covariance matrix
            for(i_P=0,k_P=0;i_P<n_P;i_P++){
              P_i_bar_accum[i_P]+=P_last[i_P];
              for(j_P=0;j_P<n_P;j_P++,k_P++)
                P_ij_bar_accum[k_P]+=P_last[i_P]*P_last[j_P];
            }

            // Compute parameter extrema and averages
            for(i_P=0;i_P<n_P;i_P++){
              if(P_last[i_P]<P_min[i_P]) P_min[i_P]=P_last[i_P];
              if(P_last[i_P]>P_max[i_P]) P_max[i_P]=P_last[i_P];
              P_avg[i_P]+=P_last[i_P];
            }

            // Compute mapping-space extrema and averages
            for(i_DS=0;i_DS<n_DS;i_DS++){
              for(i_M=0;i_M<n_M[i_DS];i_M++){
                if(M_last[i_DS][i_M]<M_min[i_DS][i_M]) M_min[i_DS][i_M]=M_last[i_DS][i_M];
                if(M_last[i_DS][i_M]>M_max[i_DS][i_M]) M_max[i_DS][i_M]=M_last[i_DS][i_M];
                M_avg[i_DS][i_M]+=M_last[i_DS][i_M];
              }
            }
          }
        }
      }
      // Finish averages
      for(i_P=0;i_P<n_P;i_P++)
        P_avg[i_P]/=(double)n_used;
      for(i_DS=0;i_DS<n_DS;i_DS++){
        for(i_M=0;i_M<n_M[i_DS];i_M++)
          M_avg[i_DS][i_M]/=(double)n_used;
      }
      rewind(fp_chain);
      for(i_iteration=0,flag_init=TRUE;i_iteration<n_iterations;i_iteration++){
        for(i_avg=0;i_avg<n_avg;i_avg++){
          fread(&flag_success,sizeof(char),  1,  fp_chain);
          fread(&ln_Pr_last,  sizeof(double),1,  fp_chain);
          fread(P_new,        sizeof(double),n_P,fp_chain);
          for(i_DS=0;i_DS<n_DS;i_DS++)
            fread(M_new[i_DS],sizeof(double),n_M[i_DS],fp_chain);
          if(i_iteration>=n_iterations_burn){
            if(flag_init || flag_success){
              memcpy(P_last,P_new,(size_t)n_P*sizeof(double));      
              for(i_DS=0;i_DS<n_DS;i_DS++)
                memcpy(M_last[i_DS],M_new[i_DS],(size_t)n_M[i_DS]*sizeof(double));
              flag_init=FALSE;
            }
            // Build coverage maps
            for(i_P=0,i_coverage=0;i_P<n_P;i_P++){
              bin_x=(double)(coverage_size)*(P_new[i_P]-P_min[i_P])/(P_max[i_P]-P_min[i_P]);
              for(j_P=i_P+1;j_P<n_P;j_P++,i_coverage++){
                bin_y=(double)(coverage_size)*(P_new[j_P]-P_min[j_P])/(P_max[j_P]-P_min[j_P]);
                if(bin_x>=0 && bin_x<coverage_size && bin_y>=0 && bin_y<coverage_size){
                  switch(flag_success){
                  case TRUE:
                    coverage_true[i_coverage][bin_x*coverage_size+bin_y]++;
                    break;
                  case FALSE:
                    coverage_false[i_coverage][bin_x*coverage_size+bin_y]++;
                    break;
                  default:
                    SID_trap_error("Unknown success flag (%d) when constructing coverage map.",ERROR_LOGIC,(int)flag_success);
                    break;
                  }
                }
              }
            }
            for(i_P=0,i_coverage=0;i_P<n_P;i_P++){
              bin_x=(double)(coverage_size)*(P_last[i_P]-P_min[i_P])/(P_max[i_P]-P_min[i_P]);
              for(j_P=i_P+1;j_P<n_P;j_P++,i_coverage++){
                bin_y=(double)(coverage_size)*(P_last[j_P]-P_min[j_P])/(P_max[j_P]-P_min[j_P]);
                if(bin_x>=0 && bin_x<coverage_size && bin_y>=0 && bin_y<coverage_size)
                  coverage_keep[i_coverage][bin_x*coverage_size+bin_y]++;
              }
            }

            // Build histograms
            for(i_P=0,i_coverage=0;i_P<n_P;i_P++){
              bin_x=(double)(coverage_size)*(P_last[i_P]-P_min[i_P])/(P_max[i_P]-P_min[i_P]);
              if(bin_x>=0 && bin_x<coverage_size)
                P_histogram[i_P][bin_x]++;
            }
            for(i_DS=0;i_DS<n_DS;i_DS++){
              for(i_M=0;i_M<n_M[i_DS];i_M++){
                bin_x=(double)(coverage_size)*(M_last[i_DS][i_M]-M_min[i_DS][i_M])/(M_max[i_DS][i_M]-M_min[i_DS][i_M]);
                if(bin_x>=0 && bin_x<coverage_size)
                  M_histogram[i_DS][i_M][bin_x]++;
              }
            }
      
            // Compute parameter standard deviations
            for(i_P=0;i_P<n_P;i_P++){
              dP_avg[i_P]+=pow(P_last[i_P]-P_avg[i_P],2.);
            }
            for(i_DS=0;i_DS<n_DS;i_DS++){
              for(i_M=0;i_M<n_M[i_DS];i_M++)
                dM_avg[i_DS][i_M]+=pow(M_last[i_DS][i_M]-M_avg[i_DS][i_M],2.);
            }
          }
        }
      } // i_iteration
      fclose(fp_chain);

      // Finish standard deviations
      for(i_P=0;i_P<n_P;i_P++)
        dP_avg[i_P]=sqrt(dP_avg[i_P]/(double)n_used);
      for(i_DS=0;i_DS<n_DS;i_DS++){
       for(i_M=0;i_M<n_M[i_DS];i_M++)
         dM_avg[i_DS][i_M]=sqrt(dM_avg[i_DS][i_M]/(double)n_used);
      }
      SID_log("Done.",SID_LOG_CLOSE);    

      // Finish covariance matrix
      for(i_P=0;i_P<n_P;i_P++)
        P_i_bar[i_P]=P_i_bar_accum[i_P]/(double)n_used;
      for(i_P=0,k_P=0;i_P<n_P;i_P++){
        for(j_P=0;j_P<n_P;j_P++,k_P++){
          P_ij_bar[k_P] =P_ij_bar_accum[k_P]/(double)n_used;
          V_compute[k_P]=P_ij_bar[k_P]-P_i_bar[i_P]*P_i_bar[j_P];
        }
      }

      // Write covariance matrix
      if(SID.I_am_Master){
        fp_covariance=fopen(filename_covariance,"wb");
        fwrite(&n_P,     sizeof(int),   1,      fp_covariance);
        fwrite(V_compute,sizeof(double),n_P*n_P,fp_covariance);
        fclose(fp_covariance);
      }

      // Compute mapped dataset confidence intervals from histograms
      SID_log("Compute confidence intervals...",SID_LOG_OPEN);
      for(i_DS=0;i_DS<n_DS;i_DS++){
        for(i_M=0;i_M<n_M[i_DS];i_M++){
          merge_sort(M_histogram[i_DS][i_M],(size_t)coverage_size,&histogram_index,SID_SIZE_T,SORT_COMPUTE_INDEX,FALSE);
          accumulator       =M_histogram[i_DS][i_M][histogram_index[coverage_size-1]];
          M_best_index      =histogram_index[coverage_size-1];
          M_lo_68_index     =histogram_index[coverage_size-1];
          M_hi_68_index     =histogram_index[coverage_size-1];
          for(j_P=coverage_size-2;j_P>=0 && ((double)accumulator/(double)n_used)<0.68;j_P--){
            if(histogram_index[j_P]<M_lo_68_index) M_lo_68_index=histogram_index[j_P];
            if(histogram_index[j_P]>M_hi_68_index) M_hi_68_index=histogram_index[j_P];
            accumulator+=M_histogram[i_DS][i_M][histogram_index[j_P]];
          }
          M_lo_95_index=M_lo_68_index;
          M_hi_95_index=M_hi_68_index;
          for(;j_P>=0 && ((double)accumulator/(double)n_used)<0.95;j_P--){
            if(histogram_index[j_P]<M_lo_95_index) M_lo_95_index=histogram_index[j_P];
            if(histogram_index[j_P]>M_hi_95_index) M_hi_95_index=histogram_index[j_P];
            accumulator+=M_histogram[i_DS][i_M][histogram_index[j_P]];
          }
          M_best[i_DS][i_M] =M_min[i_DS][i_M]+(M_max[i_DS][i_M]-M_min[i_DS][i_M])*((double)(M_best_index)+0.5)/(double)coverage_size;
          M_lo_68[i_DS][i_M]=M_min[i_DS][i_M]+(M_max[i_DS][i_M]-M_min[i_DS][i_M])*((double)(M_lo_68_index)+0.5)/(double)coverage_size;
          M_hi_68[i_DS][i_M]=M_min[i_DS][i_M]+(M_max[i_DS][i_M]-M_min[i_DS][i_M])*((double)(M_hi_68_index)+0.5)/(double)coverage_size;
          M_lo_95[i_DS][i_M]=M_min[i_DS][i_M]+(M_max[i_DS][i_M]-M_min[i_DS][i_M])*((double)(M_lo_95_index)+0.5)/(double)coverage_size;
          M_hi_95[i_DS][i_M]=M_min[i_DS][i_M]+(M_max[i_DS][i_M]-M_min[i_DS][i_M])*((double)(M_hi_95_index)+0.5)/(double)coverage_size;
          SID_free(SID_FARG histogram_index);
        }
      }

      // Compute parameter confidence intervals from histograms
      for(i_P=0;i_P<n_P;i_P++){
        merge_sort(P_histogram[i_P],(size_t)coverage_size,&histogram_index,SID_SIZE_T,SORT_COMPUTE_INDEX,FALSE);
        accumulator       =P_histogram[i_P][histogram_index[coverage_size-1]];
        P_best_index      =histogram_index[coverage_size-1];
        P_lo_68_index     =histogram_index[coverage_size-1];
        P_hi_68_index     =histogram_index[coverage_size-1];
        for(j_P=coverage_size-2;j_P>=0 && ((double)accumulator/(double)n_used)<0.68;j_P--){
          if(histogram_index[j_P]<P_lo_68_index) P_lo_68_index=histogram_index[j_P];
          if(histogram_index[j_P]>P_hi_68_index) P_hi_68_index=histogram_index[j_P];
          accumulator+=P_histogram[i_P][histogram_index[j_P]];
        }
        P_lo_95_index=P_lo_68_index;
        P_hi_95_index=P_hi_68_index;
        for(;j_P>=0 && ((double)accumulator/(double)n_used)<0.95;j_P--){
          if(histogram_index[j_P]<P_lo_95_index) P_lo_95_index=histogram_index[j_P];
          if(histogram_index[j_P]>P_hi_95_index) P_hi_95_index=histogram_index[j_P];
          accumulator+=P_histogram[i_P][histogram_index[j_P]];
        }
        P_best[i_P] =P_min[i_P]+(P_max[i_P]-P_min[i_P])*((double)(P_best_index)+0.5)/(double)coverage_size;
        P_lo_68[i_P]=P_min[i_P]+(P_max[i_P]-P_min[i_P])*((double)(P_lo_68_index)+0.5)/(double)coverage_size;
        P_hi_68[i_P]=P_min[i_P]+(P_max[i_P]-P_min[i_P])*((double)(P_hi_68_index)+0.5)/(double)coverage_size;
        P_lo_95[i_P]=P_min[i_P]+(P_max[i_P]-P_min[i_P])*((double)(P_lo_95_index)+0.5)/(double)coverage_size;
        P_hi_95[i_P]=P_min[i_P]+(P_max[i_P]-P_min[i_P])*((double)(P_hi_95_index)+0.5)/(double)coverage_size;
        SID_free(SID_FARG histogram_index);
      }

      // Compute parameter confidence contours from coverage maps
      for(i_coverage=0;i_coverage<n_coverage;i_coverage++){
        merge_sort(coverage_keep[i_coverage],(size_t)(coverage_size*coverage_size),&coverage_keep_index,SID_SIZE_T,SORT_COMPUTE_INDEX,FALSE);
        accumulator             =coverage_keep[i_coverage][coverage_keep_index[coverage_size*coverage_size-1]];
        P_contour_68[i_coverage]=coverage_keep[i_coverage][coverage_keep_index[coverage_size*coverage_size-1]];
        for(j_P=(coverage_size*coverage_size)-2;j_P>=0 && ((double)accumulator/(double)n_used)<0.68;j_P--){
          P_contour_68[i_coverage] =coverage_keep[i_coverage][coverage_keep_index[j_P]];
          accumulator             +=coverage_keep[i_coverage][coverage_keep_index[j_P]];
        }
        P_contour_95[i_coverage]=P_contour_68[i_coverage];
        for(;j_P>=0 && ((double)accumulator/(double)n_used)<0.95;j_P--){
          P_contour_95[i_coverage] =coverage_keep[i_coverage][coverage_keep_index[j_P]];
          accumulator             +=coverage_keep[i_coverage][coverage_keep_index[j_P]];
        }
        SID_free(SID_FARG coverage_keep_index);
      }
      SID_log("Done.",SID_LOG_CLOSE);

      // Write coverage maps
      SID_log("Writing coverage maps...",SID_LOG_OPEN);
      fp_coverage=fopen(filename_coverage,"wb");
      fwrite(&n_coverage,   sizeof(int),     1,fp_coverage);
      fwrite(&coverage_size,sizeof(int),     1,fp_coverage);
      fwrite(P_min,         sizeof(double),n_P,fp_coverage);
      fwrite(P_max,         sizeof(double),n_P,fp_coverage);
      for(i_P=0,i_coverage=0;i_P<n_P;i_P++){
        for(j_P=i_P+1;j_P<n_P;j_P++,i_coverage++){
          fwrite(&P_contour_68[i_coverage], sizeof(double),1,                          fp_coverage);
          fwrite(&P_contour_95[i_coverage], sizeof(double),1,                          fp_coverage);
          fwrite(coverage_true[i_coverage], sizeof(size_t),coverage_size*coverage_size,fp_coverage);
          fwrite(coverage_false[i_coverage],sizeof(size_t),coverage_size*coverage_size,fp_coverage);
          fwrite(coverage_keep[i_coverage], sizeof(size_t),coverage_size*coverage_size,fp_coverage);
        }
      }
      fclose(fp_coverage);
      SID_log("Done.",SID_LOG_CLOSE);

      // Write coverage maps
      SID_log("Writing histograms...",SID_LOG_OPEN);
      fp_histograms=fopen(filename_histograms,"wb");
      for(i_P=0;i_P<n_P;i_P++){
        fwrite(&P_best[i_P],    sizeof(double),1,            fp_histograms);
        fwrite(&P_lo_68[i_P],   sizeof(double),1,            fp_histograms);
        fwrite(&P_hi_68[i_P],   sizeof(double),1,            fp_histograms);
        fwrite(&P_lo_95[i_P],   sizeof(double),1,            fp_histograms);
        fwrite(&P_hi_95[i_P],   sizeof(double),1,            fp_histograms);
        fwrite(P_histogram[i_P],sizeof(size_t),coverage_size,fp_histograms);
      }
      for(i_DS=0;i_DS<n_DS;i_DS++){
        for(i_M=0;i_M<n_M[i_DS];i_M++){
          fwrite(&M_best[i_DS][i_M],    sizeof(double),1,            fp_histograms);
          fwrite(&M_lo_68[i_DS][i_M],   sizeof(double),1,            fp_histograms);
          fwrite(&M_hi_68[i_DS][i_M],   sizeof(double),1,            fp_histograms);
          fwrite(&M_lo_95[i_DS][i_M],   sizeof(double),1,            fp_histograms);
          fwrite(&M_hi_95[i_DS][i_M],   sizeof(double),1,            fp_histograms);
          fwrite(M_histogram[i_DS][i_M],sizeof(size_t),coverage_size,fp_histograms);
        }
      }
      fclose(fp_histograms);
      SID_log("Done.",SID_LOG_CLOSE);

      // Write results
      SID_log("Writing final statistics...",SID_LOG_OPEN);
      sprintf(filename_results,"%s/fit_for_parameters.dat",filename_results_dir);
      fp_results=fopen(filename_results,"w");
      fprintf(fp_results,"# MCMC parameter fit results to %s\n",MCMC->problem_name);
      fprintf(fp_results,"#   n_samples_used=%d\n",n_used);
      fprintf(fp_results,"#   n_parameters  =%d\n",n_P);
      fprintf(fp_results,"#   n_data_sets   =%d\n",n_DS);
      fprintf(fp_results,"#   used data sets:\n");
      current_DS=MCMC->DS;
      while(current_DS!=NULL){
        next_DS=current_DS->next;
        fprintf(fp_results,"#     %s\n",current_DS->name);
        current_DS=next_DS;
        i_DS++;
      }
      sprintf(column_txt,"Column:");
      i_column=1;
      fprintf(fp_results,"# %s (%02d) Parameter name\n",               column_txt,i_column++);
      sprintf(column_txt,"       ");
      for(i_array=0;i_array<MCMC->n_arrays;i_array++)
        fprintf(fp_results,"# %s (%02d) %s\n",                         column_txt,i_column++,MCMC->array_name[i_array]);
      fprintf(fp_results,"# %s (%02d) Initial value\n",                column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Average\n",                      column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Standard deviation\n",           column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Maximum likelihood value\n",     column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Lower limit (68%% confidence)\n",column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Upper limit (68%% confidence)\n",column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Lower limit (95%% confidence)\n",column_txt,i_column++);
      fprintf(fp_results,"# %s (%02d) Upper limit (95%% confidence)\n",column_txt,i_column++);
      for(i_P=0;i_P<n_P;i_P++){
        strcpy(array_name_temp,MCMC->P_names[i_P]);
        search_and_replace(array_name_temp," ","~");
        fprintf(fp_results,MCMC->P_name_format,array_name_temp);
        for(i_array=0;i_array<MCMC->n_arrays;i_array++)
          fprintf(fp_results," %13.6le",MCMC->array[i_array][i_P]);
        fprintf(fp_results," %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le\n",MCMC->P_init[i_P],P_avg[i_P],dP_avg[i_P],P_best[i_P],P_lo_68[i_P],P_hi_68[i_P],P_lo_95[i_P],P_hi_95[i_P]);
      }
      fclose(fp_results);
      current_DS=MCMC->DS;
      i_DS      =0;
      while(current_DS!=NULL){
        next_DS  =current_DS->next;
        M_target =current_DS->M_target;
        dM_target=current_DS->dM_target;
        if(n_DS>1)
          sprintf(filename_results,"%s/fit_for_dataset_%05d.dat",filename_results_dir,i_DS);
        else
          sprintf(filename_results,"%s/fit_for_dataset.dat",filename_results_dir);
        fp_results=fopen(filename_results,"w");
        fprintf(fp_results,"# MCMC data set fit results for %s\n",current_DS->name);
        fprintf(fp_results,"#   n_samples_used=%d\n",n_used);
        fprintf(fp_results,"#   dataset_DoF   =%d\n",n_M[i_DS]);
        sprintf(column_txt,"Column:");
        for(i_array=0,i_column=1;i_array<MCMC->n_arrays;i_array++){
          fprintf(fp_results,"# %s (%02d) %s\n",                         column_txt,i_column++,current_DS->array_name[i_array]);
          sprintf(column_txt,"       ");
        }
        sprintf(column_txt,"       ");
        fprintf(fp_results,"# %s (%02d) Dataset\n",                      column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Dataset uncertainty\n",          column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Average\n",                      column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Standard deviation\n",           column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Maximum likelihood value\n",     column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Lower limit (68%% confidence)\n",column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Upper limit (68%% confidence)\n",column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Lower limit (95%% confidence)\n",column_txt,i_column++);
        fprintf(fp_results,"# %s (%02d) Upper limit (95%% confidence)\n",column_txt,i_column++);
        for(i_P=0;i_P<n_M[i_DS];i_P++){
          for(i_array=0;i_array<n_M_arrays[i_DS];i_array++)
            fprintf(fp_results,"%13.6le ",M_arrays[i_DS][i_array][i_P]);
          fprintf(fp_results,"%13.6le %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le %13.6le\n",M_target[i_P],dM_target[i_P],M_avg[i_DS][i_P],dM_avg[i_DS][i_P],M_best[i_DS][i_P],M_lo_68[i_DS][i_P],M_hi_68[i_DS][i_P],M_lo_95[i_DS][i_P],M_hi_95[i_DS][i_P]);
        }
        fclose(fp_results);    
        current_DS=next_DS;
        i_DS++;
      }    
      SID_log("Done.",SID_LOG_CLOSE);
    }

  // Clean-up
  SID_log("Cleaning-up...",SID_LOG_OPEN);
  SID_free(SID_FARG P_i_bar_accum);
  SID_free(SID_FARG P_ij_bar_accum);
  SID_free(SID_FARG P_i_bar);
  SID_free(SID_FARG P_ij_bar);
  SID_free(SID_FARG V_compute);
  for(i_DS=0;i_DS<n_DS;i_DS++){
    SID_free(SID_FARG M_new[i_DS]);
    SID_free(SID_FARG M_last[i_DS]);
    SID_free(SID_FARG M_avg[i_DS]);
    SID_free(SID_FARG dM_avg[i_DS]);
    SID_free(SID_FARG M_best[i_DS]);
    SID_free(SID_FARG M_min[i_DS]);
    SID_free(SID_FARG M_max[i_DS]);
    SID_free(SID_FARG M_lo_68[i_DS]);
    SID_free(SID_FARG M_hi_68[i_DS]);
    SID_free(SID_FARG M_lo_95[i_DS]);
    SID_free(SID_FARG M_hi_95[i_DS]);
    for(j_M=0;j_M<n_M[i_DS];j_M++)
      SID_free(SID_FARG M_histogram[i_DS][j_M]);
    SID_free(SID_FARG M_histogram[i_DS]);
  }
  SID_free(SID_FARG n_M);
  SID_free(SID_FARG M_new);
  SID_free(SID_FARG M_last);
  SID_free(SID_FARG M_avg);
  SID_free(SID_FARG dM_avg);
  SID_free(SID_FARG M_best);
  SID_free(SID_FARG M_min);
  SID_free(SID_FARG M_max);
  SID_free(SID_FARG M_lo_68);
  SID_free(SID_FARG M_hi_68);
  SID_free(SID_FARG M_lo_95);
  SID_free(SID_FARG M_hi_95);
  SID_free(SID_FARG M_histogram);
  SID_free(SID_FARG P_new);
  SID_free(SID_FARG P_last);
  SID_free(SID_FARG P_min);
  SID_free(SID_FARG P_max);
  SID_free(SID_FARG P_avg);
  SID_free(SID_FARG dP_avg);
  SID_free(SID_FARG P_contour_68);
  SID_free(SID_FARG P_contour_95);
  SID_free(SID_FARG P_best);
  SID_free(SID_FARG P_lo_68);
  SID_free(SID_FARG P_hi_68);
  SID_free(SID_FARG P_lo_95);
  SID_free(SID_FARG P_hi_95);
  for(i_P=0;i_P<n_P;i_P++)
    SID_free(SID_FARG P_histogram[i_P]);
  SID_free(SID_FARG P_histogram);
  SID_free(SID_FARG n_M_arrays);
  SID_free(SID_FARG M_arrays);
  SID_free(SID_FARG M_avg);
  SID_free(SID_FARG dM_avg);
  for(i_P=0;i_P<n_coverage;i_P++){
    SID_free(SID_FARG coverage_true[i_P]);
    SID_free(SID_FARG coverage_false[i_P]);
    SID_free(SID_FARG coverage_keep[i_P]);
  }
  SID_free(SID_FARG coverage_true);
  SID_free(SID_FARG coverage_false);
  SID_free(SID_FARG coverage_keep);


  SID_log("Done.",SID_LOG_CLOSE);

  SID_log("Done.",SID_LOG_CLOSE);
}
