#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

void add_to_trees_horizontal_stats(tree_horizontal_stats_info *stats, int id, int descendant_id, int n_particles, int type) {
    // Find maximum id
    stats->max_id = GBP_MAX(stats->max_id, id);

    // Compute statistcs for mergers
    if(descendant_id >= 0 && id != descendant_id)
        stats->n_mergers++;

    // Compute statistcs for strays
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_STRAYED)) {
        stats->max_strayed_size = GBP_MAX(stats->max_strayed_size, n_particles);
        stats->n_strayed++;
    }

    // Compute statistcs for dropped halos
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_DROPPED)) {
        stats->max_dropped_size = GBP_MAX(stats->max_dropped_size, n_particles);
        stats->n_dropped++;
    }

    // Compute statistcs for bridged and emerged halos
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_BRIDGED)) {
        stats->max_bridged_size = GBP_MAX(stats->max_bridged_size, n_particles);
        stats->n_bridged++;
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_MATCHED_TO_EMERGED)) {
        stats->max_emerged_progenitor_size = GBP_MAX(stats->max_emerged_progenitor_size, n_particles);
        stats->n_emerged_progenitors++;
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_EMERGED)) {
        stats->max_emerged_size = GBP_MAX(stats->max_emerged_size, n_particles);
        stats->n_emerged++;
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_STRAYED)) {
        stats->max_fragmented_strayed_size = GBP_MAX(stats->max_fragmented_strayed_size, n_particles);
        stats->n_fragmented_strayed++;
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_NORMAL)) {
        stats->max_fragmented_normal_size = GBP_MAX(stats->max_fragmented_normal_size, n_particles);
        stats->n_fragmented_normal++;
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_OTHER)) {
        stats->max_fragmented_other_size = GBP_MAX(stats->max_fragmented_other_size, n_particles);
        stats->n_fragmented_other++;
    }

    // Sanity checks
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_INVALID)) {
        stats->n_invalid++;
        if((type - TREE_CASE_INVALID) != 0)
            SID_exit_error("An out-of-bounds halo has been manipulated (type=%d)", SID_ERROR_LOGIC, type);
    }
    if(SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_UNPROCESSED))
        stats->n_unprocessed++;
}
