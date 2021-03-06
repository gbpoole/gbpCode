#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees.h>

int main(int argc, char *argv[]) {
    char        filename_SSimPL_dir[256];
    char        filename_SSimPL_base[256];
    char        filename_halo_version_root[256];
    char        filename_trees_name[256];
    char        filename_halo_root_in[256];
    char        filename_cat_root_in[256];
    char        filename_root_matches[256];
    char        filename_root_out[256];
    char        filename_snap_list_in[256];
    char        filename_snap_list_out[256];
    char        filename_output_file_root[256];
    int         i_read_start;
    int         i_read_stop;
    int         i_read_step;
    int         n_search;
    int         n_files_subgroups;
    int         n_k_match               = 2;
    int         flag_clean              = GBP_FALSE;
    double      f_match_moment_diff_min = F_MATCH_MOMENT_DIFF_MIN_DEFAULT;
    FILE *      fp_in;
    FILE *      fp_out;
    char *      line        = NULL;
    size_t      line_length = 0;
    int         n_snaps, i_read, i_next, i_write, n_keep;
    double *    a_list_in;
    cosmo_info *cosmo;
    int         flag_fix_bridges = GBP_TRUE;

    SID_Init(&argc, &argv, NULL);

    // Fetch user inputs
    strcpy(filename_SSimPL_dir, argv[1]);
    strcpy(filename_halo_version_root, argv[2]);
    strcpy(filename_trees_name, argv[3]);
    i_read_start     = atoi(argv[4]);
    i_read_stop      = atoi(argv[5]);
    i_read_step      = atoi(argv[6]);
    n_search         = atoi(argv[7]);
    flag_fix_bridges = atoi(argv[8]);
    if(argc == 10)
        f_match_moment_diff_min = atof(argv[9]);

    // Report the run time parameters
    SID_log("Constructing horizontal merger trees...", SID_LOG_OPEN | SID_LOG_TIMER);
    SID_log("Run parameters:", SID_LOG_OPEN);
    SID_log("Simulation root: {%s}", SID_LOG_COMMENT, filename_SSimPL_dir);
    SID_log("Halo version:    {%s}", SID_LOG_COMMENT, filename_halo_version_root);
    SID_log("Tree version:    {%s}", SID_LOG_COMMENT, filename_trees_name);
    SID_log("Snapshots:       %d->%d", SID_LOG_COMMENT, i_read_start, i_read_stop);
    SID_log("Step/n_search:   %d/%d", SID_LOG_COMMENT, i_read_step, n_search);
    SID_log("", SID_LOG_CLOSE | SID_LOG_NOPRINT);

    // Set some paths etc
    sprintf(filename_SSimPL_base, "%s", filename_SSimPL_dir);
    strip_path(filename_SSimPL_base);
    sprintf(filename_halo_root_in, "%s/halos/%s", filename_SSimPL_dir, filename_halo_version_root);
    sprintf(filename_cat_root_in, "%s/catalogs/%s", filename_SSimPL_dir, filename_halo_version_root);
    sprintf(filename_root_matches, "%s/trees/matches", filename_SSimPL_dir);
    sprintf(filename_snap_list_in, "%s/run/a_list.txt", filename_SSimPL_dir);
    sprintf(filename_root_out, "%s/trees/%s", filename_SSimPL_dir, filename_trees_name);

    // Initialize cosmology
    char filename_cosmology[SID_MAX_FILENAME_LENGTH];
    sprintf(filename_cosmology, "%s/run", filename_SSimPL_dir);
    read_gbpCosmo_file(&cosmo, filename_cosmology);

    // Generate trees
    compute_trees_horizontal(filename_halo_root_in,
                             filename_cat_root_in,
                             filename_snap_list_in,
                             filename_root_matches,
                             filename_root_out,
                             &cosmo,
                             i_read_start,
                             i_read_stop,
                             i_read_step,
                             n_search,
                             flag_fix_bridges,
                             f_match_moment_diff_min,
                             &flag_clean);

    // Clean-up
    free_cosmo(&cosmo);
    SID_free(SID_FARG line);

    SID_log("Done.", SID_LOG_CLOSE);
    SID_Finalize();
}
