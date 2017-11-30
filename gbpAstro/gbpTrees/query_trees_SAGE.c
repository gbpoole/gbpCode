#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees.h>

int main(int argc, char *argv[]) {
    SID_Init(&argc, &argv, NULL);

    // Fetch user inputs
    if(argc != 7 && argc != 6)
        SID_exit_error("Invalid Syntax.", SID_ERROR_SYNTAX);
    char filename_SSimPL_root[SID_MAX_FILENAME_LENGTH];
    char filename_halos_root[SID_MAX_FILENAME_LENGTH];
    char filename_trees_root[SID_MAX_FILENAME_LENGTH];
    char group_prefix[8];
    strcpy(filename_SSimPL_root, argv[1]);
    strcpy(filename_halos_root, argv[2]);
    strcpy(filename_trees_root, argv[3]);
    if(!strcmp(argv[4], "groups") || !strcmp(argv[4], "group")) {
        sprintf(group_prefix, "");
    } else if(!strcmp(argv[4], "subgroups") || !strcmp(argv[4], "subgroup")) {
        sprintf(group_prefix, "sub");
    } else {
        SID_exit_error("Invalid mode selection {%s}.  Should be 'group' or 'subgroup'.", SID_ERROR_SYNTAX, argv[4]);
    }
    int i_forest_query = atoi(argv[5]);

    // Loop over the files until we find the tree we're looking for
    int i_forest_lo   = 0;
    int i_forest_hi   = -1;
    int i_file        = 0;
    int flag_continue = GBP_TRUE;
    do {
        SID_log("Scanning file %d...", SID_LOG_OPEN, i_file);

        // Open file
        char filename_in[SID_MAX_FILENAME_LENGTH];
        sprintf(filename_in, "%s/trees/%s/vertical/%sgroup_trees_%03d.dat", filename_SSimPL_root, filename_trees_root, group_prefix, i_file);
        FILE *fp_in_trees = fopen(filename_in, "r");

        // Check that the file has been opened
        if(fp_in_trees == NULL)
            SID_exit_error("File {%s} could not be opened while still scanning for forest %d.", SID_ERROR_IO_OPEN,
                           filename_in, i_forest_query);

        // Read header
        int n_forest_file = 0;
        int n_halo_file   = 0;
        SID_fread_verify(&n_forest_file, sizeof(int), 1, fp_in_trees);
        SID_fread_verify(&n_halo_file, sizeof(int), 1, fp_in_trees);

        // Set this file's bounds
        i_forest_lo = i_forest_hi + 1;
        i_forest_hi = i_forest_lo + n_forest_file - 1;

        // Check if the tree we're looking for is in this file.  If it isn't, skip to the next file
        if(i_forest_query >= i_forest_lo && i_forest_query <= i_forest_hi) {
            // Finish reading the header
            int *n_halo_forest_file = (int *)SID_malloc(sizeof(int) * n_forest_file);
            SID_fread_verify(n_halo_forest_file, sizeof(int), n_forest_file, fp_in_trees);

            // Scan up-to the desired forest
            int j_forest = 0;
            int i_forest = i_forest_lo;
            for(; i_forest < i_forest_query; i_forest++)
                fseeko(fp_in_trees, sizeof(halo_properties_SAGE_info) * n_halo_forest_file[j_forest++], SEEK_CUR);

            // Read the forest we're querying
            int                        n_halo_forest_query = n_halo_forest_file[j_forest];
            halo_properties_SAGE_info *forest = (halo_properties_SAGE_info *)SID_malloc(sizeof(halo_properties_SAGE_info) * n_halo_forest_query);
            SID_fread_verify(forest, sizeof(halo_properties_SAGE_info), n_halo_forest_query, fp_in_trees);
            SID_log("%d halos read.", SID_LOG_COMMENT, n_halo_forest_query, i_forest);

            // Process the forest
            for(int i_halo = 0; i_halo < n_halo_forest_query; i_halo++)
                fprintf(stdout,
                        "%d -- %6d -- d:%6d p1:%6d pn:%6d g1:%6d gn:%6d\n",
                        i_halo,
                        forest[i_halo].n_particles,
                        forest[i_halo].descendant,
                        forest[i_halo].progenitor_first,
                        forest[i_halo].progenitor_next,
                        forest[i_halo].group_halo_first,
                        forest[i_halo].group_halo_next);

            // Clean-up
            SID_free(SID_FARG n_halo_forest_file);
            SID_free(SID_FARG forest);
            flag_continue = GBP_FALSE;
        }
        // Move to the next file
        else {
            SID_log("skipping halos %d->%d.", SID_LOG_CLOSE, i_forest_lo, i_forest_hi);
            i_file++;
        }
        // Close file
        fclose(fp_in_trees);
    } while(flag_continue); // loop over files

    SID_log("Done.", SID_LOG_CLOSE);
    SID_Finalize();
}
