#ifndef GBPLIB_AWAKE
#define GBPLIB_AWAKE

#include <gbpSID.h>
#include <gbpSort.h>
#include <gbpCalc.h>
#include <gbpADaPS.h>
#include <gbpParse.h>
#if USE_CFITSIO
#include <gbpFITS.h>
#endif
#if USE_HDF5
#include <gbpHDF5.h>
#endif
#include <gbpMultifile.h>

// Compile with -DSID_CONSTANTS_OFF
// to prevent the inclusion of these
// constants, if collisions with other
// projects is occuring.
#include <gbpConstants.h>

#endif
