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

int fetch_treenode_snap_tree(tree_info *trees, tree_node_info *halo) {
    if(halo != NULL)
        return (halo->snap_tree);
    return (-1);
}