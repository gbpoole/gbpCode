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

int find_treenode_main_progenitor(tree_info *trees, tree_node_info *halo, tree_node_info **main_progenitor) {
    tree_node_info *branch_root;
    (*main_progenitor) = NULL;
    if(halo != NULL) {
        // Find the last snapshot of this halo
        if(find_treenode_last_snapshot(trees, halo, &branch_root)) {
            if(branch_root != NULL) {
                // Search for the main progenitor
                tree_node_info *track_halo  = branch_root->descendant;
                int             target_snap = halo->snap_tree;
                while(track_halo != NULL) {
                    if(track_halo->snap_tree == target_snap)
                        break;
                    track_halo = track_halo->progenitor_first;
                }
                if(track_halo != NULL) {
                    // n.b.: It is possible that a halo is it's own main progenitor
                    //       at this point.
                    if(track_halo->snap_tree == target_snap) {
                        (*main_progenitor) = track_halo;
                        return (TRUE);
                    } else
                        return (FALSE);
                } else
                    return (FALSE);
            } else
                return (FALSE);
        } else
            return (FALSE);
    }
    return (FALSE);
}
