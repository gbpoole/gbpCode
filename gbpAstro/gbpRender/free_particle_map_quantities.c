#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void free_particle_map_quantities(map_quantities_info *mq){
   SID_free(SID_FARG mq->h_smooth);
   SID_free(SID_FARG mq->x);
   SID_free(SID_FARG mq->y);
   SID_free(SID_FARG mq->z);
   SID_free(SID_FARG mq->rho);
   SID_free(SID_FARG mq->sigma);
   SID_free(SID_FARG mq->ptype_used);
}
