#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_linear_theory.h>

double dDplus_da(double a, cosmo_info **cosmo) {
    interp_info *D_a_interp = NULL;
    if(!ADaPS_exist(*cosmo, "D_a_interp")) {
        int     n_a    = 100;
        double  da     = 0.;
        double *a_temp = (double *)SID_malloc(sizeof(double) * n_a);
        double *D_temp = (double *)SID_malloc(sizeof(double) * n_a);
        init_array_linear(1e-6, 1., n_a, &a_temp, &da);
        for(int i_a = 0; i_a < n_a; i_a++)
            D_temp[i_a] = Dplus(a_temp[i_a], cosmo);
        init_interpolate(a_temp, D_temp, (size_t)n_a, gsl_interp_cspline, &D_a_interp);
        ADaPS_store_interp(cosmo, (void *)D_a_interp, "D_a_interp");
        SID_free(SID_FARG a_temp);
        SID_free(SID_FARG D_temp);
    } else
        D_a_interp = (interp_info *)ADaPS_fetch(*cosmo, "D_a_interp");
    return (interpolate_derivative(D_a_interp, a));
}
