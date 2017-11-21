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

int check_treenode_if_fragmented(tree_node_info *halo) {
    if(halo != NULL)
        return (SID_CHECK_BITFIELD_SWITCH(halo->tree_case, TREE_CASE_FRAGMENTED_STRAYED) ||
                SID_CHECK_BITFIELD_SWITCH(halo->tree_case, TREE_CASE_FRAGMENTED_NORMAL) ||
                SID_CHECK_BITFIELD_SWITCH(halo->tree_case, TREE_CASE_FRAGMENTED_OTHER));
    return (GBP_FALSE);
}
