#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>
#include <gbpTrees_analysis.h>

void free_precompute_treenode_markers(tree_info *trees, int mode) {
    // Set-up to work with groups or subgroups
    tree_markers_info **markers;
    if(SID_CHECK_BITFIELD_SWITCH(mode, PRECOMPUTE_TREENODE_MARKER_GROUPS))
        markers = trees->group_markers;
    else if(SID_CHECK_BITFIELD_SWITCH(mode, PRECOMPUTE_TREENODE_MARKER_SUBGROUPS))
        markers = trees->subgroup_markers;
    else
        SID_exit_error("Neither group nor subgroup mode is set in init_treenode_markers_all() when one is needed.",
                       SID_ERROR_LOGIC);

    // Perform deallocation
    if(markers != NULL) {
        for(int i_snap = 0; i_snap < trees->n_snaps; i_snap++)
            SID_free(SID_FARG markers[i_snap]);
    }
    SID_free(SID_FARG markers);

    // Process reference trees if present
    if(trees->trees_reference != NULL)
        free_precompute_treenode_markers(trees->trees_reference, mode);
}
