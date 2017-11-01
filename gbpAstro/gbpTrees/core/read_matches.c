#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

void read_matches(char *  filename_in_dir,
                  int     i_read_in,
                  int     j_read_in,
                  int     n_halos_max,
                  int     mode,
                  int *   n_groups_i,
                  int *   n_groups_j,
                  int *   n_particles_i_in,
                  int *   n_particles_j_in,
                  int *   n_sub_group_i_in,
                  int *   n_sub_group_j_in,
                  int *   match_ids,
                  float * match_score,
                  int *   match_count,
                  size_t *match_index,
                  char *  match_flag_two_way,
                  double  f_match_moment_diff_min,
                  int     flag_reject_bad_matches) {
    char   group_text_prefix[5];
    char   filename_in[SID_MAX_FILENAME_LENGTH];
    SID_fp fp_in;
    int    k_read;
    int    l_read;
    int    n_search;
    int    n_matches;
    size_t offset;
    int    flag_continue;
    int    i_read_file;
    int    j_read_file;
    int    n_groups_file;
    int    n_groups_file_1;
    int    n_groups_file_2;
    int    n_groups;
    int    n_groups_i_file;
    int    n_groups_j_file;
    int *  n_sub_group_i;
    int *  n_sub_group_j;
    int    flag_alloc_n_sub_i = GBP_FALSE;
    int    flag_alloc_n_sub_j = GBP_FALSE;

    if(i_read_in == j_read_in)
        SID_exit_error("i_read=j_read in read_matches", SID_ERROR_LOGIC);

    switch(mode) {
        case MATCH_SUBGROUPS:
            sprintf(group_text_prefix, "sub");
            break;
        case MATCH_GROUPS:
            sprintf(group_text_prefix, "");
            // We need n_subgroups arrays for removal of bad groups
            //    if they have not been passed to us.
            if(n_sub_group_i_in == NULL) {
                flag_alloc_n_sub_i = GBP_TRUE;
                n_sub_group_i      = (int *)SID_malloc(sizeof(int) * n_halos_max);
            } else
                n_sub_group_i = n_sub_group_i_in;
            if(n_sub_group_j_in == NULL) {
                flag_alloc_n_sub_j = GBP_TRUE;
                n_sub_group_j      = (int *)SID_malloc(sizeof(int) * n_halos_max);
            } else
                n_sub_group_j = n_sub_group_j_in;
            break;
    }

    // Since we need the particle counts for the goodness of match criterion,
    //   create temporary arrays in case we weren't passed an array for them.
    int *n_particles_i = n_particles_i_in;
    int *n_particles_j = n_particles_j_in;
    // 1) We always need n_particles_i
    int flag_alloc_n_particles_i = GBP_FALSE;
    int flag_alloc_n_particles_j = GBP_FALSE;
    if(n_particles_i == NULL)
        flag_alloc_n_particles_i = GBP_TRUE;
    if(n_particles_j == NULL)
        flag_alloc_n_particles_j = GBP_TRUE;
    // 2) We need n_particles_j if doing 2-way checks
    if(match_flag_two_way != NULL && n_particles_j == NULL)
        flag_alloc_n_particles_j = GBP_TRUE;

    // Read the needed info from the header file
    int   i_read;
    int   i_read_start;
    int   i_read_stop;
    int   n_search_total;
    int   n_files;
    int   n_groups_in;
    int   counter = 0;
    char  filename_in_name[256];
    float match_weight_rank_index;
    strcpy(filename_in_name, filename_in_dir);
    strip_path(filename_in_name);
    sprintf(filename_in, "%s/%sgroup_matches_header.dat", filename_in_dir, group_text_prefix);
    SID_fopen(filename_in, "r", &fp_in);
    SID_fread(&i_read_start, sizeof(int), 1, &fp_in);
    SID_fread(&i_read_stop, sizeof(int), 1, &fp_in);
    SID_fread(&n_search_total, sizeof(int), 1, &fp_in);
    SID_fread(&n_files, sizeof(int), 1, &fp_in);
    for(i_read = i_read_stop; i_read >= i_read_start && counter < 2; i_read--) {
        SID_fread(&i_read_file, sizeof(int), 1, &fp_in);
        if(i_read_file == i_read_in) {
            SID_fread(n_groups_i, sizeof(int), 1, &fp_in);
            if((*n_groups_i) > 0) {
                // Create a temporary array for n_particles_i if we were not passed one
                if(flag_alloc_n_particles_i)
                    n_particles_i = (int *)SID_malloc(sizeof(int) * (*n_groups_i));
                if(n_particles_i != NULL)
                    SID_fread_ordered(n_particles_i, sizeof(int), (size_t)(*n_groups_i), &fp_in);
                else
                    SID_fskip(sizeof(int), (*n_groups_i), &fp_in);
                if(mode == MATCH_GROUPS) {
                    if(n_sub_group_i != NULL)
                        SID_fread_ordered(n_sub_group_i, sizeof(int), (size_t)(*n_groups_i), &fp_in);
                    else
                        SID_fskip(sizeof(int), (*n_groups_i), &fp_in);
                }
            }
            counter++;
        } else if(i_read_file == j_read_in) {
            SID_fread(n_groups_j, sizeof(int), 1, &fp_in);
            if((*n_groups_j) > 0) {
                if(flag_alloc_n_particles_j)
                    n_particles_j = (int *)SID_malloc(sizeof(int) * (*n_groups_j));
                if(n_particles_j != NULL)
                    SID_fread_ordered(n_particles_j, sizeof(int), (size_t)(*n_groups_j), &fp_in);
                else
                    SID_fskip(sizeof(int), (*n_groups_j), &fp_in);
                if(mode == MATCH_GROUPS) {
                    if(n_sub_group_j != NULL)
                        SID_fread_ordered(n_sub_group_j, sizeof(int), (size_t)(*n_groups_j), &fp_in);
                    else
                        SID_fskip(sizeof(int), (*n_groups_j), &fp_in);
                }
            }
            counter++;
        } else {
            SID_fread(&n_groups_in, sizeof(int), 1, &fp_in);
            if(n_groups_in > 0) {
                SID_fskip(sizeof(int), n_groups_in, &fp_in);
                if(mode == MATCH_GROUPS)
                    SID_fskip(sizeof(int), n_groups_in, &fp_in);
            }
        }
    }
    SID_fclose(&fp_in);

    // Read the matching file
    char filename_cat1[256];
    char filename_cat2[256];
    char filename_in_dir_snap[256];
    sprintf(filename_cat1, "%03d", i_read_in);
    sprintf(filename_cat2, "%03d", j_read_in);
    sprintf(filename_in_dir_snap, "%s/%s", filename_in_dir, filename_cat1);
    if(filename_in_dir != NULL)
        sprintf(filename_in, "%s/%sgroup_matches_%s_%s.dat", filename_in_dir_snap, group_text_prefix, filename_cat1, filename_cat2);
    else
        sprintf(filename_in, "%s_%sgroup_matches_%s_%s.dat", filename_in_name, group_text_prefix, filename_cat1, filename_cat2);

    SID_fopen(filename_in, "r", &fp_in);
    SID_fread(&i_read_file, sizeof(int), 1, &fp_in);
    SID_fread(&j_read_file, sizeof(int), 1, &fp_in);
    SID_fread(n_groups_i, sizeof(int), 1, &fp_in);
    SID_fread(n_groups_j, sizeof(int), 1, &fp_in);
    SID_fread(&match_weight_rank_index, sizeof(float), 1, &fp_in);

    // Read matching data
    SID_fread(match_ids, sizeof(int), (*n_groups_i), &fp_in);
    SID_fread(match_score, sizeof(float), (*n_groups_i), &fp_in);
    SID_fread(match_count, sizeof(int), (*n_groups_i), &fp_in);
    SID_fclose(&fp_in);

    // If one of the catalogs is empty, set to no-match defaults
    if((*n_groups_i) <= 0 || (*n_groups_j) <= 0) {
        for(int i_halo = 0; i_halo < (*n_groups_i); i_halo++) {
            match_ids[i_halo]   = -1;
            match_score[i_halo] = 0.;
            match_count[i_halo] = 0;
        }
        if(match_index != NULL) {
            for(int i_halo = 0; i_halo < (*n_groups_i); i_halo++)
                match_index[i_halo] = (size_t)i_halo;
        }
        if(match_flag_two_way != NULL) {
            for(int i_halo = 0; i_halo < (*n_groups_i); i_halo++)
                match_flag_two_way[i_halo] = GBP_FALSE;
        }
    } else {
        // If we are reading groups, nullify all matches
        //    between halos with no substructures.
        int i_halo;
        if(mode == MATCH_GROUPS) {
            for(i_halo = 0; i_halo < (*n_groups_i); i_halo++) {
                if(n_sub_group_i[i_halo] <= 0) {
                    match_ids[i_halo]   = -1;
                    match_score[i_halo] = 0.;
                    match_count[i_halo] = 0;
                } else if(match_ids[i_halo] >= 0 && (*n_groups_j) > 0) {
                    if(n_sub_group_j[match_ids[i_halo]] <= 0) {
                        match_ids[i_halo]   = -1;
                        match_score[i_halo] = 0.;
                        match_count[i_halo] = 0;
                    }
                }
            }
        }

        // Apply a goodness-of-fit criterion and check that the maximum allowed score has not been exceeded
        for(i_halo = 0; i_halo < (*n_groups_i); i_halo++) {
            if(match_ids[i_halo] >= 0) {
                if(flag_reject_bad_matches &&
                   !check_validity_of_match(n_particles_i[i_halo], match_count[i_halo], match_score[i_halo], f_match_moment_diff_min))
                    match_ids[i_halo] = -1;
            }
        }

        // Since we may have changed some matches with the goodness
        //    of fit criterion, we need to re-perform the sort
        if(match_index != NULL) {
            size_t *match_index_temp = NULL;
            merge_sort(match_ids, (size_t)(*n_groups_i), &match_index_temp, SID_INT, SORT_COMPUTE_INDEX, SORT_COMPUTE_NOT_INPLACE);
            memcpy(match_index, match_index_temp, (*n_groups_i) * sizeof(size_t));
            SID_free(SID_FARG match_index_temp);
        }

        // Determine if the matches are two-way if we have been asked to check this
        if(match_flag_two_way != NULL && (*n_groups_i) > 0) {
            // We're going to need the particle counts in the target catalog for checking the goodness of return matches.  Make sure we have them.
            if(n_particles_j == NULL)
                SID_exit_error(
                        "Target catalog halo sizes are not defined in read_matches() but are needed for checking two-way match flags.",
                        SID_ERROR_LOGIC);

            // Flip the file names for reading the return matches
            sprintf(filename_in_dir_snap, "%s/%s", filename_in_dir, filename_cat2);
            if(filename_in_dir != NULL)
                sprintf(filename_in, "%s/%sgroup_matches_%s_%s.dat", filename_in_dir_snap, group_text_prefix, filename_cat2, filename_cat1);
            else
                sprintf(filename_in, "%s_%sgroup_matches_%s_%s.dat", filename_in_name, group_text_prefix, filename_cat2, filename_cat1);

            // Open two files, one for reading the IDs and one for matching the scores of the matching file
            int    i_read_file_check;
            int    j_read_file_check;
            int    n_groups_i_check;
            int    n_groups_j_check;
            float  match_score_rank_index;
            SID_fp fp_check_ids;
            SID_fp fp_check_score;
            SID_fp fp_check_count;
            SID_fopen(filename_in, "r", &fp_check_ids);
            SID_fopen(filename_in, "r", &fp_check_score);
            SID_fopen(filename_in, "r", &fp_check_count);
            SID_fread(&j_read_file_check, sizeof(int), 1, &fp_check_ids);
            SID_fread(&i_read_file_check, sizeof(int), 1, &fp_check_ids);
            SID_fread(&n_groups_j_check, sizeof(int), 1, &fp_check_ids);
            SID_fread(&n_groups_i_check, sizeof(int), 1, &fp_check_ids);
            SID_fread(&match_score_rank_index, sizeof(float), 1, &fp_check_ids);

            // Check that we have the right files
            if(n_groups_i_check != (*n_groups_i))
                SID_exit_error("Source halo counts don't match (ie. %d!=%d) in two-way check in read_matches().",
                               SID_ERROR_LOGIC, n_groups_i_check, (*n_groups_i));
            if(n_groups_j_check != (*n_groups_j))
                SID_exit_error("Target halo counts don't match (ie. %d!=%d) in two-way check in read_matches().",
                               SID_ERROR_LOGIC, n_groups_j_check, (*n_groups_j));
            if(i_read_file_check != i_read_file)
                SID_exit_error("Source file numbers don't match (ie. %d!=%d) in two-way check in read_matches().",
                               SID_ERROR_LOGIC, i_read_file_check, i_read_file);
            if(j_read_file_check != j_read_file)
                SID_exit_error("Target file numbers don't match (ie. %d!=%d) in two-way check in read_matches().",
                               SID_ERROR_LOGIC, j_read_file_check, j_read_file);

            // Skip to the beginning of the relevant block for the score-reading file pointer
            SID_fskip(sizeof(int), 4, &fp_check_score);             // header
            SID_fskip(sizeof(float), 1, &fp_check_score);           // header
            SID_fskip(sizeof(int), (*n_groups_j), &fp_check_score); // ids

            // Skip to the beginning of the relevant block for the count-reading file pointer
            SID_fskip(sizeof(int), 4, &fp_check_count);               // header
            SID_fskip(sizeof(float), 1, &fp_check_count);             // header
            SID_fskip(sizeof(int), (*n_groups_j), &fp_check_count);   // ids
            SID_fskip(sizeof(float), (*n_groups_j), &fp_check_count); // scores

            // Set everything to being a one-way match unless subsequently changed
            for(i_halo = 0; i_halo < (*n_groups_i); i_halo++)
                match_flag_two_way[i_halo] = GBP_FALSE;

            // Read matching data in buffered chunks
            int    n_good       = 0;
            int    n_2way       = 0;
            int    n_buffer     = 1024 * 1024;
            int    n_chunk      = 0;
            int    n_remaining  = (*n_groups_j);
            int *  buffer_ids   = (int *)SID_malloc(sizeof(int) * n_buffer);
            float *buffer_score = (float *)SID_malloc(sizeof(float) * n_buffer);
            int *  buffer_count = (int *)SID_malloc(sizeof(int) * n_buffer);
            for(int j_halo = 0; n_remaining > 0; n_remaining -= n_chunk) {
                n_chunk = GBP_MIN(n_remaining, n_buffer);
                SID_fread(buffer_ids, sizeof(int), n_chunk, &fp_check_ids);
                SID_fread(buffer_score, sizeof(float), n_chunk, &fp_check_score);
                SID_fread(buffer_count, sizeof(int), n_chunk, &fp_check_count);
                for(int k_halo = 0; k_halo < n_chunk; k_halo++, j_halo++) {
                    int id_i = buffer_ids[k_halo];
                    if(id_i >= 0) {
                        if(id_i >= (*n_groups_i))
                            SID_exit_error(
                                    "Allowed matching index has been exceeded i(ie %d>=%d) while determining two-way match flags.",
                                    SID_ERROR_LOGIC, id_i, (*n_groups_i));
                        // Do this check first to avoid having to check if both needed n_particles_* references are defined
                        int id_j = match_ids[id_i];
                        if(id_j == j_halo) {
                            if(!flag_reject_bad_matches ||
                               check_validity_of_match(n_particles_j[j_halo], buffer_count[k_halo], buffer_score[k_halo], f_match_moment_diff_min)) {
                                match_flag_two_way[id_i] = GBP_TRUE;
                                n_2way++;
                            }
                            n_good++;
                        }
                    }
                }
            }
            // SID_log("n_good=%d n_2way=%d",SID_LOG_COMMENT,n_good,n_2way);
            SID_fclose(&fp_check_ids);
            SID_fclose(&fp_check_score);
            SID_fclose(&fp_check_count);
            SID_free(SID_FARG buffer_ids);
            SID_free(SID_FARG buffer_score);
            SID_free(SID_FARG buffer_count);
        }
    }

    // If any of these arrays are temporary, free them.
    if(flag_alloc_n_sub_i)
        SID_free(SID_FARG n_sub_group_i);
    if(flag_alloc_n_sub_j)
        SID_free(SID_FARG n_sub_group_j);
    if(flag_alloc_n_particles_i)
        SID_free(SID_FARG n_particles_i);
    if(flag_alloc_n_particles_j)
        SID_free(SID_FARG n_particles_j);
}
