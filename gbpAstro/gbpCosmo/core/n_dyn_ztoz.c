#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_core.h>

double dn_dyn_dt_local(double a, void *cosmo_in);
double dn_dyn_dt_local(double a, void *cosmo_in) {
    cosmo_info *cosmo = (cosmo_info *)cosmo_in;
    return (1. / (a * H_convert(H_z(z_of_a(a), cosmo)) * t_dyn_z(z_of_a(a), cosmo)));
}

double delta_n_dyn_local(double a_1, double a_2, cosmo_info **cosmo);
double delta_n_dyn_local(double a_1, double a_2, cosmo_info **cosmo) {
    // Initialize integral
    double                     integral;
    int                        n_int        = 512;
    double                     abs_accuracy = 1e-4;
    double                     rel_accuracy = 1e-4;
    double                     abs_error;
    double                     a_lo = MIN(a_1, a_2);
    double                     a_hi = MAX(a_1, a_2);
    gsl_integration_workspace *wspace;
    gsl_function               integrand;
    integrand.function = dn_dyn_dt_local;
    integrand.params   = (void *)(*cosmo);
    wspace             = gsl_integration_workspace_alloc(16 * n_int);
    gsl_integration_qag(&integrand, a_lo, a_hi, abs_accuracy, rel_accuracy, n_int, GSL_INTEG_GAUSS61, wspace, &integral, &abs_error);
    gsl_integration_workspace_free(wspace);
    return (integral);
}

double n_dyn_ztoz(double redshift_1, double redshift_2, cosmo_info *cosmo) {
    double redshift_lo = MIN(redshift_1, redshift_2);
    double redshift_hi = MAX(redshift_1, redshift_2);
    return (delta_n_dyn_local(a_of_z(redshift_hi), a_of_z(redshift_lo), &cosmo));
}
