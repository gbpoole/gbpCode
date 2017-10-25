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

double fetch_treenode_xi(tree_info *trees, tree_node_info *halo) {
    double xi = -1;
    if(halo != NULL) {
        tree_node_info *secondary = halo;
        tree_node_info *primary   = halo->descendant->progenitor_primary;
        fetch_treenode_merger_info(trees, &secondary, &primary, &xi, NULL, NULL, NULL);
    }
    return (xi);
}
