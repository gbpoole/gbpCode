#define _MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees.h>

int main(int argc, char *argv[]) {
    int    n_search;
    int    i_halo;
    char   filename_in[SID_MAX_FILENAME_LENGTH];
    char   group_text_prefix[4];
    int    n_files;
    int    k_read;
    int    max_n_groups;
    int    l_read;
    int    n_groups;
    int *  n_particles_i;
    int *  n_particles_j;
    int    j_read;
    int    mode;
    int    n_groups_i;
    int    n_groups_j;
    int    j_halo;
    int    i_read;
    int    i_read_start;
    int    i_read_stop;
    SID_fp fp_in;

    SID_init(&argc, &argv, NULL, NULL);

    // Fetch user inputs
    char filename_root_in[SID_MAX_FILENAME_LENGTH];
    char filename_catalog_root[SID_MAX_FILENAME_LENGTH];
    char filename_halo_version[SID_MAX_FILENAME_LENGTH];
    strcpy(filename_root_in, argv[1]);
    strcpy(filename_halo_version, argv[2]);
    if(!strcmp(argv[3], "groups") || !strcmp(argv[3], "group"))
        mode = MATCH_GROUPS;
    else if(!strcmp(argv[3], "subgroups") || !strcmp(argv[3], "subgroup"))
        mode = MATCH_SUBGROUPS;
    else {
        SID_log("Invalid mode selection {%s}.  Should be 'group' or 'subgroup'.", SID_LOG_COMMENT, argv[3]);
        SID_exit(SID_ERROR_SYNTAX);
    }
    i_read               = atoi(argv[4]);
    j_read               = atoi(argv[5]);
    int flag_SSimPL_base = GBP_TRUE;
    if(argc == 7) {
        flag_SSimPL_base = GBP_FALSE;
        strcpy(filename_catalog_root, argv[6]);
        sprintf(filename_catalog_root, "%s/halos/%s", argv[6], filename_halo_version);
    } else
        sprintf(filename_catalog_root, "%s/halos/%s", filename_root_in, filename_halo_version);
    SID_log("Searching match information for halo #%d in file #%d of {%s}...", SID_LOG_OPEN | SID_LOG_TIMER, i_halo, i_read, filename_root_in);

    // Convert filename_root to filename
    switch(mode) {
        case MATCH_SUBGROUPS:
            sprintf(group_text_prefix, "sub");
            break;
        case MATCH_GROUPS:
            sprintf(group_text_prefix, "");
            break;
    }

    // Set the standard SSiMPL match file path
    char filename_root[SID_MAX_FILENAME_LENGTH];
    if(flag_SSimPL_base)
        sprintf(filename_root, "%s/trees/matches/%03d/", filename_root_in, i_read);
    else
        sprintf(filename_root, "%s_", filename_root_in);

    // Read header information
    int   i_read_in;
    int   j_read_in;
    int   n_groups_1;
    int   n_groups_2;
    float score_rank_index;
    sprintf(filename_in, "%s%sgroup_matches_%03d_%03d.dat", filename_root, group_text_prefix, i_read, j_read);
    SID_fopen(filename_in, "r", &fp_in);
    SID_fread(&i_read_in, sizeof(int), 1, &fp_in);
    SID_log("i_read    =%d", SID_LOG_COMMENT, i_read_in);
    SID_fread(&j_read_in, sizeof(int), 1, &fp_in);
    SID_log("j_read    =%d", SID_LOG_COMMENT, j_read_in);
    SID_fread(&n_groups_i, sizeof(int), 1, &fp_in);
    SID_log("n_groups_i=%d", SID_LOG_COMMENT, n_groups_i);
    SID_fread(&n_groups_j, sizeof(int), 1, &fp_in);
    SID_log("n_groups_j=%d", SID_LOG_COMMENT, n_groups_j);
    SID_fread(&score_rank_index, sizeof(int), 1, &fp_in);
    SID_log("score_idx =%f", SID_LOG_COMMENT, score_rank_index);

    // Allocate RAM
    int *  match = (int *)SID_malloc(sizeof(int) * n_groups_i);
    float *score = (float *)SID_malloc(sizeof(float) * n_groups_i);
    int *  count = (int *)SID_malloc(sizeof(int) * n_groups_i);

    // Read arrays
    SID_fread(match, sizeof(int), n_groups_i, &fp_in);
    SID_fread(score, sizeof(float), n_groups_i, &fp_in);
    SID_fread(count, sizeof(int), n_groups_i, &fp_in);

    // Close file
    SID_fclose(&fp_in);

    // Read halo sizes from header file
    SID_log("Reading halo sizes...", SID_LOG_OPEN);
    int *n_p_i = (int *)SID_malloc(sizeof(int) * n_groups_i);
    sprintf(filename_in, "%s_%03d.catalog_%sgroups", filename_catalog_root, i_read, group_text_prefix);
    SID_fopen(filename_in, "r", &fp_in);
    int n_cat_i;
    int offset_size_i;
    SID_fread(&n_cat_i, sizeof(int), 1, &fp_in);
    SID_fread(&offset_size_i, sizeof(int), 1, &fp_in);
    if(n_cat_i != n_groups_i)
        SID_trap_error("Catalog 'i' halo counts don't match (ie %d!=%d)", SID_ERROR_LOGIC, n_cat_i, n_groups_i);
    SID_fread(n_p_i, sizeof(int), n_cat_i, &fp_in);
    SID_fclose(&fp_in);

    int *n_p_j = (int *)SID_malloc(sizeof(int) * n_groups_j);
    sprintf(filename_in, "%s_%03d.catalog_%sgroups", filename_catalog_root, j_read, group_text_prefix);
    SID_fopen(filename_in, "r", &fp_in);
    int n_cat_j;
    int offset_size_j;
    SID_fread(&n_cat_j, sizeof(int), 1, &fp_in);
    SID_fread(&offset_size_j, sizeof(int), 1, &fp_in);
    if(n_cat_j != n_groups_j)
        SID_trap_error("Catalog 'i' halo counts don't match (ie %d!=%d)", SID_ERROR_LOGIC, n_cat_j, n_groups_j);
    SID_fread(n_p_j, sizeof(int), n_cat_j, &fp_in);
    SID_fclose(&fp_in);
    SID_log("Done.", SID_LOG_CLOSE);

    // Print results
    for(k_read = 0; k_read < n_groups_i; k_read++) {
        if(match[k_read] >= 0)
            printf("%7d %7d %7d %7d %7d %le %le %le\n",
                   k_read,
                   match[k_read],
                   n_p_i[k_read],
                   n_p_j[match[k_read]],
                   count[k_read],
                   score[k_read],
                   maximum_match_score(n_p_i[k_read]),
                   match_score_f_goodness(score[k_read], n_p_i[k_read]));
    }

    // Clean-up
    SID_free(SID_FARG match);
    SID_free(SID_FARG score);
    SID_free(SID_FARG count);
    SID_free(SID_FARG n_p_i);
    SID_free(SID_FARG n_p_j);

    SID_log("Done.", SID_LOG_CLOSE);
    SID_exit(SID_ERROR_NONE);
}
