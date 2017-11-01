#ifndef GBPCALC_H
#define GBPCALC_H

#define CALC_MODE_DEFAULT DEFAULT_MODE
#define CALC_MODE_RETURN_DOUBLE TTTP00
#define CALC_MODE_ABS TTTP01

#define CALC_STAT_DEFAULT DEFAULT_MODE
#define CALC_STAT_RETURN_DOUBLE TTTP00
#define CALC_STAT_GLOBAL TTTP01
#define CALC_STAT_SUM TTTP02
#define CALC_STAT_MIN TTTP03
#define CALC_STAT_MAX TTTP04
#define CALC_STAT_MEAN TTTP05
#define CALC_STAT_MEDIAN TTTP06
#define CALC_STAT_STDDEV TTTP07

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif

void calc_max(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_max_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
void calc_mean(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_mean_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
void calc_min(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_min_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
void calc_stddev(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_stddev_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
void calc_sum(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_sum_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);
void calc_median(void *data, void *result, size_t n_data, SID_Datatype type, int mode);
void calc_median_global(void *data_local, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm);

#ifdef __cplusplus
}
#endif
#endif
