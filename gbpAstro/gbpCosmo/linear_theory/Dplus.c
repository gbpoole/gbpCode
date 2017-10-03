#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo_linear_theory.h>

double Dplus(double a,cosmo_info **cosmo){
   return(Dplus_unnormed(a,cosmo)/Dplus_unnormed(1.,cosmo));
}

