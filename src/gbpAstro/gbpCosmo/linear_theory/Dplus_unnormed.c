#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_linear_theory.h>

double Dplus_integrand_local(double a, void *cosmo) {
    return (1.0 / pow(a * H_z(z_of_a(a), (cosmo_info *)cosmo), 3.0)); // See, eg., Eqn A6 of Neisenstein & Dekel 2008
}

double Dplus_unnormed(double a, cosmo_info **cosmo) {
    // Initialize integral
    int                        n_int        = 1000;
    double                     rel_accuracy = 1e-8;
    double                     abs_error;
    double                     r_val;
    double                     limit_lo = 0.;
    double                     limit_hi = a;
    gsl_function               integrand;
    gsl_integration_workspace *wspace;
    integrand.function = Dplus_integrand_local;
    integrand.params   = (void *)(*cosmo);
    wspace             = gsl_integration_workspace_alloc(n_int);

    // Perform integral
    gsl_integration_qags(&integrand, limit_lo, limit_hi, 1e-5, 0., n_int, wspace, &r_val, &abs_error); // use qags for singularity at a=0

    // Clean-up
    gsl_integration_workspace_free(wspace);

    // Apply coefficients and return result
    r_val *= H_z(z_of_a(a), *cosmo);

    return (r_val);
}
