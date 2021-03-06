#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_core.h>
#include <gbpCosmo_NFW_etc.h>
#include <gsl/gsl_sf_expint.h>

// From Alam et al '02
double R_half_V_max_NFW(double M_vir, double z, int mode, cosmo_info **cosmo) {
    double c_vir;
    double R_vir;
    double r_o;
    double r_val = 0.;

    set_NFW_params(M_vir, z, mode, cosmo, &c_vir, &R_vir);

    r_o   = R_vir / c_vir;
    r_val = 0.13 * r_o;

    return (r_val);
}
