#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_linear_theory.h>

double merger_rate_B_n(cosmo_info **cosmo, int mode_model, double z, double M_0, double xi) {
    double B_n       = -1.;
    int    flag_done = FALSE;

    // Fakhouri & Ma 2008, eqn 10
    if(mode_model == 0 || mode_model == 1) {
        flag_done        = TRUE;
        int    mode      = PSPEC_LINEAR_TF;
        int    component = PSPEC_ALL_MATTER;
        double M_prime;
        // Option 'A' in Fakhouri et al 2008
        if(mode_model == 0) {
            M_prime = M_0 * xi / (1. + xi);
        }
        // Option 'B' in Fakhouri et al 2008
        else if(mode_model == 1) {
            M_prime = M_0 / (1. + xi);
        }
        double a = a_of_z(z);
        // double ddc_dz           =1.260-0.1134*pow(1.+z,-2.)-0.3654*exp(-1.16*z); // From taking the z-derivative of Eqn A7 in Neisenstein and
        // Dekel, 2008
        double ddc_dz              = -1.686 * pow(Dplus(a, cosmo), -2.) * dDplus_dz(z, cosmo);
        double sigma_M_0           = sigma_M(cosmo, M_0, z, mode, component);
        double sigma_M_prime       = sigma_M(cosmo, M_prime, z, mode, component);
        double dlnsigma_dlnM_prime = fabs(dln_sigma_dlnM(cosmo, M_prime, z, mode, component));
        B_n                        = sqrt(2. / PI) * ddc_dz * (1. / sigma_M_prime) * dlnsigma_dlnM_prime *
              pow(1. - ((sigma_M_0 * sigma_M_0) / (sigma_M_prime * sigma_M_prime)), -1.5);
        // Option 'A' currently not working.  Just return zeros.
        if(mode_model == 0)
            B_n = 0.;
    }
    // Fakhouri+ 2010
    if(mode_model == 2) {
        flag_done      = TRUE;
        double A       = 0.0104;
        double xi_tild = 9.72e-3;
        double alpha   = 0.133;
        double beta    = -1.995;
        double gamma   = 0.263;
        double eta     = 0.0993;
        B_n            = A * pow(M_0 / (1e12 * M_SOL), alpha) * pow(xi, beta) * exp(pow(xi / xi_tild, gamma)) * pow(1 + z, eta);
    }
    // Poole+ 2017
    if(mode_model == 3) {
        flag_done      = TRUE;
        double A       = 0.107;
        double xi_tild = 0.475;
        double alpha   = 0.121;
        double beta    = -1.752;
        double gamma   = 0.671;
        double eta     = 0.0643;
        B_n            = A * pow(M_0 / (1e12 * M_SOL), alpha) * pow(xi, beta) * exp(pow(xi / xi_tild, gamma)) * pow(1 + z, eta);
    }
    // Fakhouri & Ma 2008, eqn 10
    if(mode_model == 4) {
        flag_done      = TRUE;
        double A       = 0.0289;
        double xi_tild = 0.098;
        double alpha   = 0.083;
        double beta    = -2.01;
        double gamma   = 0.409;
        double eta     = 0.371;
        B_n            = A * pow(M_0 / (1e12 * M_SOL), alpha) * pow(xi, beta) * exp(pow(xi / xi_tild, gamma)) * pow(1 + z, eta);
    }
    if(!flag_done)
        SID_trap_error("Mode flag (%d) is invalid in merger_rate_B_n().  No model definition.", ERROR_LOGIC, mode_model);
    return (B_n);
}
