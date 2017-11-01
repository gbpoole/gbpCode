/**********************/
/* Cosmology routines */
/**********************/
#ifndef GBPCOSMO_MASS_FUNCTIONS_AWAKE
#define GBPCOSMO_MASS_FUNCTIONS_AWAKE

#include <gbpCosmo_core.h>
#include <gbpCosmo_linear_theory.h>

#define MF_PASS_PARAMS SID_TTTP01
#define MF_JENKINS SID_TTTP02
#define MF_PS SID_TTTP03
#define MF_ST SID_TTTP04
#define MF_WATSON SID_TTTP05
#define MF_TIAMAT SID_TTTP06

// Function definitions
#ifdef __cplusplus
extern "C" {
#endif
double mass_function(double M_interp, double z, cosmo_info **cosmo, int select_flag, ...);
double mass_function_cumulative(double M_interp, double z, cosmo_info **cosmo, int select_flag, ...);
double scaled_mass_function(double sigma, int mode, double *P);
#ifdef __cplusplus
}
#endif
#endif
