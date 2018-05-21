#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <gbpSID_file_io.h>
#include <gbpSwapEndian.h>

// Define the arguments for this application
struct arg_list {
    char filename_SSimPL_in[SID_MAX_FILENAME_LENGTH];
    char filename_SSimPL_out[SID_MAX_FILENAME_LENGTH];
    char filename_halo_type[SID_MAX_FILENAME_LENGTH];
    int  flag_process_halos;
    int  flag_process_catalogs;
    int  flag_process_grids;
    int  flag_process_snapshots;
    int  mode;
    int  i_snap_lo;
    int  i_snap_hi;
};
// This include sets-up the application with the
// structure given above intended to be used to
// store all needed cmdl arguments and options.
#include <gbpSID_app.hh>
using namespace gbpSID;

// Build command-line parser for this application.
// Note that options for help and the executable name
// are added to the parser by default.  No need to
// worry about either of those.
void application::define_arguments() {
    // Set default values
    args = {"unset","unset","unset", GBP_TRUE, GBP_TRUE, GBP_TRUE, GBP_TRUE, SWAP_SSIMPL_ENDIAN_TO_NATIVE, 1, 1};
    // Define options
    cli |= clara::Opt([&](bool) { args.flag_process_halos = GBP_FALSE; })["--ignore-halos"]("Ignore processing of halo files");
    cli |= clara::Opt([&](bool) { args.flag_process_catalogs = GBP_FALSE; })["--ignore-catalogs"]("Ignore processing of catalog files");
    cli |= clara::Opt([&](bool) { args.flag_process_grids = GBP_FALSE; })["--ignore-grids"]("Ignore processing of grid files");
    cli |= clara::Opt([&](bool) { args.flag_process_snapshots = GBP_FALSE; })["--ignore-snapshots"]("Ignore processing of snapshots");
    cli |= clara::Opt([&](bool) { args.mode = SWAP_SSIMPL_ENDIAN_FROM_NATIVE; })["-f"]["--from-native"]("Convert *from* the native architecture");
    // Define arguments
    cli |= clara::Arg(args.filename_SSimPL_in, "filename_SSimPL_root")("Path to the input simulation's SSimPL root directory");
    cli |= clara::Arg(args.filename_SSimPL_out, "filename_SSimPL_root")("Path to the output simulation's SSimPL root directory");
    cli |= clara::Arg(args.filename_halo_type, "filename_halo_type")("Halo finder root name (eg. subfind)");
    cli |= clara::Arg(args.i_snap_lo, "snapshot_start")("First snapshot to process");
    cli |= clara::Arg(args.i_snap_hi, "snapshot_stop")("Last snapshot to process");
    // Add some instructions text to the syntax help
    //instructions_text += "Add text here.";
}

// This method executes the application's logic
int application::execute() {

    // Create output directories
    char dir_snapshots[SID_MAX_FILENAME_LENGTH];
    char dir_smooth[SID_MAX_FILENAME_LENGTH];
    char dir_grids[SID_MAX_FILENAME_LENGTH];
    char dir_halos[SID_MAX_FILENAME_LENGTH];
    char dir_catalogs[SID_MAX_FILENAME_LENGTH];
    sprintf(dir_snapshots, "%s/snapshots", args.filename_SSimPL_out);
    sprintf(dir_smooth, "%s/smooth", args.filename_SSimPL_out);
    sprintf(dir_grids, "%s/grids", args.filename_SSimPL_out);
    sprintf(dir_halos, "%s/halos", args.filename_SSimPL_out);
    sprintf(dir_catalogs, "%s/catalogs", args.filename_SSimPL_out);
    mkdir(args.filename_SSimPL_out, 02755);
    mkdir(dir_snapshots, 02755);
    mkdir(dir_smooth, 02755);
    mkdir(dir_grids, 02755);
    mkdir(dir_halos, 02755);
    mkdir(dir_catalogs, 02755);

    // Loop over the given snapshot range
    SID_log("Processing group/subgroup statistics for files #%d->#%d...", SID_LOG_OPEN | SID_LOG_TIMER, args.i_snap_lo, args.i_snap_hi);
    for(int i_snap = args.i_snap_lo; i_snap <= args.i_snap_hi; i_snap++) {
        char filename_in[SID_MAX_FILENAME_LENGTH];
        char filename_out[SID_MAX_FILENAME_LENGTH];
        SID_log("Processing snapshot #%03d...", SID_LOG_OPEN | SID_LOG_TIMER, i_snap);
        // Process halos
        if(args.flag_process_halos) {
            sprintf(filename_in, "%s/halos/", args.filename_SSimPL_in);
            sprintf(filename_out, "%s/halos/", args.filename_SSimPL_out);
            swap_endian_halos(filename_in, filename_out, args.filename_halo_type, i_snap, args.mode);
        }
        // Process catalogs
        if(args.flag_process_catalogs) {
            sprintf(filename_in, "%s/catalogs/", args.filename_SSimPL_in);
            sprintf(filename_out, "%s/catalogs/", args.filename_SSimPL_out);
            swap_endian_catalogs(filename_in, filename_out, args.filename_halo_type, i_snap, args.mode);
        }
        // Process grids
        if(args.flag_process_grids) {
            sprintf(filename_in, "%s/grids/snapshot_%03d_dark_grid.dat", args.filename_SSimPL_in, i_snap);
            sprintf(filename_out, "%s/grids/snapshot_%03d_dark_grid.dat", args.filename_SSimPL_out, i_snap);
            swap_endian_grids(filename_in, filename_out, args.mode);
        }
        // Process snapshots and their smooth files
        if(args.flag_process_snapshots) {
            int IDs_byte_size;
            int i_region = -1;
            if(swap_endian_snapshot(args.filename_SSimPL_in, args.filename_SSimPL_out, i_region, i_snap, args.mode, &IDs_byte_size))
                swap_endian_smooth(args.filename_SSimPL_in, args.filename_SSimPL_out, i_region, i_snap, args.mode, IDs_byte_size);
            i_region++;
            while(swap_endian_snapshot(args.filename_SSimPL_in, args.filename_SSimPL_out, i_region, i_snap, args.mode, &IDs_byte_size)) {
                swap_endian_smooth(args.filename_SSimPL_in, args.filename_SSimPL_out, i_region, i_snap, args.mode, IDs_byte_size);
                i_region++;
            }
        }
        SID_log("Done.", SID_LOG_CLOSE);
    }

    SID_log("Done.", SID_LOG_CLOSE);

    // Exit cleanly
    return (SID_ERROR_NONE);
}
