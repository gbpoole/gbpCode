#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpCosmo.h>

int main(int argc, char *argv[]) {
    SID_Init(&argc, &argv, NULL);

    // Parse arguments and initialize
    if(argc < 3 || argc > 4) {
        fprintf(stderr, "\n Syntax: %s z [gbpCosmo_file.txt]\n", argv[0]);
        fprintf(stderr, " ------\n\n");
        return (SID_ERROR_SYNTAX);
    }
    double z = (double)atof(argv[1]);
    double M = (double)atof(argv[2]);

    SID_log("Computing merger rates for z=%.2lf M=%le M_sol...", SID_LOG_OPEN, z, M);

    // Initialize cosmology
    char        cosmology_name[256];
    cosmo_info *cosmo = NULL;
    if(argc == 3) {
        init_cosmo_default(&cosmo);
        sprintf(cosmology_name, GBP_COSMOLOGY_DEFAULT);
    } else if(argc == 4) {
        read_gbpCosmo_file(&cosmo, argv[3]);
        strcpy(cosmology_name, argv[3]);
    }

    // Initialize
    int mode      = PSPEC_LINEAR_TF;
    int component = PSPEC_ALL_MATTER;
    init_sigma_M(&cosmo, mode, component);
    double h_Hubble = ((double *)ADaPS_fetch(cosmo, "h_Hubble"))[0];
    SID_log("Done.", SID_LOG_CLOSE);

    // Generate file
    int i_column = 1;
    SID_log("Writing table to stdout...", SID_LOG_OPEN);
    printf("# Merger rates for {%s} cosmology; z=%le and M=%le M_sol\n", cosmology_name, z, M);
    printf("# Column (%02d): xi\n", i_column++);
    printf("#        (%02d): Merger rate B/n(xi) (eqn 10 from Fakhouri & Ma, 2008; option A)\n", i_column++);
    printf("#        (%02d): Merger rate B/n(xi) (eqn 10 from Fakhouri & Ma, 2008; option B)\n", i_column++);
    printf("#        (%02d): Merger rate B/n(xi) (Fit to Millennium data from Fakhouri & Ma, 2010)\n", i_column++);
    printf("#        (%02d): Merger rate B/n(xi) (Fit to GiggleZ and Tiamat from Poole et al.)\n", i_column++);
    printf("#        (%02d): Merger rate B/n(xi) (Fit to Millennium data from Fakhouri+ 2008)\n", i_column++);
    for(double xi = 1.; xi > 1e-4; xi *= 0.9) {
        printf("%10.5le %10.5le %10.5le %10.5le %10.5le %10.5le\n",
               xi,
               merger_rate_B_n(&cosmo, 0, z, M * M_SOL, xi),
               merger_rate_B_n(&cosmo, 1, z, M * M_SOL, xi),
               merger_rate_B_n(&cosmo, 2, z, M * M_SOL, xi),
               merger_rate_B_n(&cosmo, 3, z, M * M_SOL, xi),
               merger_rate_B_n(&cosmo, 4, z, M * M_SOL, xi));
    }
    SID_log("Done.", SID_LOG_CLOSE);

    // Clean-up
    free_cosmo(&cosmo);

    SID_log("Done.", SID_LOG_CLOSE);
    SID_Finalize();
}
