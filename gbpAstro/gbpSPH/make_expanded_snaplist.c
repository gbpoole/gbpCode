#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>

double dn_dyn_dt(double a, void *cosmo_in);
double dn_dyn_dt(double a, void *cosmo_in) {
    cosmo_info *cosmo = (cosmo_info *)cosmo_in;
    return (1. / (a * H_convert(H_z(z_of_a(a), cosmo)) * t_dyn_z(z_of_a(a), cosmo)));
}

double delta_n_dyn(double a_1, double a_2, cosmo_info **cosmo);
double delta_n_dyn(double a_1, double a_2, cosmo_info **cosmo) {
    // Initialize integral
    double                     integral;
    int                        n_int        = 512;
    double                     abs_accuracy = 0.;
    double                     rel_accuracy = 1e-4;
    double                     abs_error;
    double                     a_lo = GBP_MIN(a_1, a_2);
    double                     a_hi = GBP_MAX(a_1, a_2);
    gsl_integration_workspace *wspace;
    gsl_function               integrand;
    integrand.function = dn_dyn_dt;
    integrand.params   = (void *)(*cosmo);
    wspace             = gsl_integration_workspace_alloc(16 * n_int);
    gsl_integration_qag(&integrand, a_lo, a_hi, abs_accuracy, rel_accuracy, n_int, GSL_INTEG_GAUSS61, wspace, &integral, &abs_error);
    gsl_integration_workspace_free(wspace);
    return (integral);
}

int main(int argc, char *argv[]) {
    SID_Init(&argc, &argv, NULL);

    // Parse arguments
    char filename_cosmo[SID_MAX_FILENAME_LENGTH];
    char filename_list_in[SID_MAX_FILENAME_LENGTH];
    strcpy(filename_cosmo, argv[1]);
    strcpy(filename_list_in, argv[2]);

    // Read input list
    char *  line        = NULL;
    size_t  line_length = 0;
    FILE *  fp_in       = fopen(filename_list_in, "r");
    int     n_list      = count_lines_data(fp_in);
    double *a_list      = (double *)SID_malloc(sizeof(double) * n_list);
    for(int i_list = 0; i_list < n_list; i_list++) {
        grab_next_line_data(fp_in, &line, &line_length);
        grab_double(line, 1, &(a_list[i_list]));
    }
    SID_free(SID_FARG line);

    double a_lo_table = a_list[0];
    double a_hi_table = a_list[n_list - 1];
    for(int i_list = 0; i_list < n_list; i_list++) {
        a_lo_table = GBP_MIN(a_lo_table, a_list[i_list]);
        a_hi_table = GBP_MAX(a_hi_table, a_list[i_list]);
    }
    a_lo_table = GBP_MIN(a_lo_table, 1e-4);

    SID_log("Creating extended snapshot list...", SID_LOG_OPEN);

    // Initialize cosmology
    cosmo_info *cosmo = NULL;
    read_gbpCosmo_file(&cosmo, filename_cosmo);

    // Creeate interpolation tables
    int     n_table     = 500;
    double *a_table     = (double *)SID_malloc(n_table * sizeof(double));
    double *t_table     = (double *)SID_malloc(n_table * sizeof(double));
    double *n_dyn_table = (double *)SID_malloc(n_table * sizeof(double));
    double  da_table    = (a_hi_table - a_lo_table) / (double)(n_table - 1);
    for(int i_table = 0; i_table < n_table; i_table++) {
        // Expansion factor
        if(i_table == 0)
            a_table[i_table] = a_lo_table;
        else if(i_table == (n_table - 1))
            a_table[i_table] = a_hi_table;
        else
            a_table[i_table] = a_table[0] + da_table * (double)i_table;
        // Cosmic time
        t_table[i_table] = t_age_a(a_table[i_table], &cosmo);
        // Number of dynamical times
        if(i_table == 0)
            n_dyn_table[i_table] = 0.;
        else
            n_dyn_table[i_table] = delta_n_dyn(a_table[0], a_table[i_table], &cosmo);
    }
    interp_info *n_of_a_interp = NULL;
    init_interpolate(a_table, n_dyn_table, n_table, gsl_interp_akima, &n_of_a_interp);

    // Generate desired table
    double a_last   = 0.;
    double z_last   = 0.;
    double t_last   = 0.;
    double n_last   = 0.;
    double a_i      = 0.;
    double z_i      = 1.e22;
    double t_i      = 0.;
    double n_i      = 0.;
    double da       = 0.;
    double dz       = 0.;
    double dt       = 0.;
    double dn       = 0.;
    int    i_column = 1;
    printf("# Table of times, etc. generated from expansion factors in {%s}\n", filename_list_in);
    printf("# Column (%02d): snapshot number\n", i_column++);
    printf("#        (%02d): a:=expansion factor\n", i_column++);
    printf("#        (%02d): da\n", i_column++);
    printf("#        (%02d): z:=redshift\n", i_column++);
    printf("#        (%02d): dz\n", i_column++);
    printf("#        (%02d): t:=cosmic time  [years]\n", i_column++);
    printf("#        (%02d): dt\n", i_column++);
    printf("#        (%02d): n_dyn:=No. of dynamical times\n", i_column++);
    printf("#        (%02d): dn_dyn\n", i_column++);
    for(int i_list = 0; i_list < n_list; i_list++) {
        a_last = a_i;
        z_last = z_i;
        t_last = t_i;
        n_last = n_i;
        a_i    = a_list[i_list];
        z_i    = z_of_a(a_i);
        t_i    = t_age_a(a_i, &cosmo) / S_PER_YEAR;
        n_i    = interpolate(n_of_a_interp, a_i);
        da     = a_i - a_last;
        dz     = fabs(z_i - z_last);
        dt     = t_i - t_last;
        dn     = n_i - n_last;
        printf("%03d %le %le %le %le %le %le %le %le\n", i_list, a_i, da, z_i, dz, t_i, dt, n_i, dn);
    }

    // Clean-up
    SID_free(SID_FARG a_list);
    SID_free(SID_FARG a_table);
    SID_free(SID_FARG t_table);
    SID_free(SID_FARG n_dyn_table);
    free_interpolate(SID_FARG n_of_a_interp, NULL);
    free_cosmo(&cosmo);

    SID_log("Done.", SID_LOG_CLOSE);
    SID_Finalize();
}
