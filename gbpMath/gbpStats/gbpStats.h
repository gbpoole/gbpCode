#ifndef GBPSTATS_AWAKE
#define GBPSTATS_AWAKE
#include <gbpLib.h>

void   calc_median(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void   calc_median_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
double calc_sep_periodic(double x_1, double y_1, double z_1, double x_2, double y_2, double z_2, double box_size);
#endif
