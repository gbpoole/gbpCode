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

float fetch_treenode_backmatch_score(tree_info *trees, tree_node_info *halo) {
    if(halo != NULL) {
        if(halo->parent_top == NULL) {
            if(trees->group_backmatch_score != NULL)
                return (trees->group_backmatch_score[halo->snap_tree][halo->neighbour_index]);
            else
                SID_exit_error("Group backmatch scores are not defined.  They probably have not been read.",
                               SID_ERROR_LOGIC);
        } else {
            if(trees->subgroup_backmatch_score != NULL)
                return (trees->subgroup_backmatch_score[halo->snap_tree][halo->neighbour_index]);
            else
                SID_exit_error("Subgroup backmatch scores are not defined.  They probably have not been read.",
                               SID_ERROR_LOGIC);
        }
    }
    return (-1.);
}
