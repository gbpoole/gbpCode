#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

int check_if_particle_marked(char **mark, int i_species, size_t i_particle, char *c_i) {
    // Return GBP_FALSE only if mark[i_species][i_particle] is defined ...
    if(mark[i_species] == NULL) {
        (*c_i) = 1; // default to white
        return (GBP_TRUE);
    }
    // ... and equal to GBP_FALSE.
    else {
        (*c_i) = mark[i_species][i_particle];
        return (!(mark[i_species][i_particle] == GBP_FALSE));
    }
}
