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

int check_treenode_if_satellite(tree_node_info *halo) {
    if(halo != NULL) {
        if(halo->parent_top != NULL)
            return (halo->parent_top->substructure_first != halo);
        else
            return (GBP_FALSE);
    }
    return (GBP_FALSE);
}
