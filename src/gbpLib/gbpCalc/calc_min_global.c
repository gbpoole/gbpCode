#include <gbpSID.h>
#include <gbpCalc.h>

void calc_min_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm) {
    calc_min(data_local, result, n_data_local, type, mode);
    if(SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_RETURN_DOUBLE))
        SID_Allreduce(SID_IN_PLACE, result, 1, SID_DOUBLE, SID_MIN, comm);
    else
        SID_Allreduce(SID_IN_PLACE, result, 1, type, SID_MIN, comm);
}
