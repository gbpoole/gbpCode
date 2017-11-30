#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <gbpLib.h>
#include <gbpSwapEndian.h>
#include <gbpSID_app.hh>

namespace gbpApp {

    // Define the argument structure for this application
    struct arg_list {
        char filename_SSimPL_in[SID_MAX_FILENAME_LENGTH];
        char filename_SSimPL_out[SID_MAX_FILENAME_LENGTH];
        char filename_halo_type[SID_MAX_FILENAME_LENGTH];
        int i_snap_lo;
        int i_snap_hi;
        int flag_process_halos;
        int flag_process_catalogs;
        int flag_process_grids;
        int flag_process_snapshots;
        int mode;
    };

    // Set default values
    arg_list args ={ GBP_FALSE,GBP_TRUE,GBP_TRUE,GBP_TRUE,GBP_TRUE,SWAP_SSIMPL_ENDIAN_TO_NATIVE,1,1};

    // Create a personalised derived class for this application from 
    // the base class defined in "gbpApp.hh".
    class application: public gbpSID::application {
        public:
            application(int argc, char **argv):gbpSID::application(argc,argv){};

            // Define command line options for this application
            void define_arguments() {
                cli |= clara::Opt([&](bool) { args.flag_process_halos = GBP_FALSE; })["--ignore-halos"]("Ignore processing of halo files")
                    |  clara::Opt([&](bool) { args.flag_process_catalogs = GBP_FALSE; })["--ignore-catalogs"]("Ignore processing of catalog files")
                    |  clara::Opt([&](bool) { args.flag_process_grids = GBP_FALSE; })["--ignore-grids"]("Ignore processing of grid files")
                    |  clara::Opt([&](bool) { args.flag_process_snapshots = GBP_FALSE; })["--ignore-snapshots"]("Ignore processing of snapshots")
                    |  clara::Opt([&](bool) { args.mode = SWAP_SSIMPL_ENDIAN_FROM_NATIVE; })["-f"]["--from-native"]("Convert *from* the native architecture")
                    |  clara::Arg(args.filename_SSimPL_in, "filename_SSimPL_root")("Path to the input simulation's SSimPL root directory")
                    |  clara::Arg(args.filename_SSimPL_out,"filename_SSimPL_root")("Path to the output simulation's SSimPL root directory")
                    |  clara::Arg(args.filename_halo_type, "filename_halo_type")("Halo finder root name (eg. subfind)")
                    |  clara::Arg(args.i_snap_lo, "snapshot_start")("First snapshot to process")
                    |  clara::Arg(args.i_snap_hi, "snapshot_stop")("Last snapshot to process");
            }

            // This method executes the application's logic
            int execute() {
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
                return(SID_ERROR_NONE);
            }
    }; 
} // namespace gbpApp

// Include main() for applications.  They must have a
// #define _MAIN declared before including gbpApp.hh
#include <gbpSID_app_main.hh>
