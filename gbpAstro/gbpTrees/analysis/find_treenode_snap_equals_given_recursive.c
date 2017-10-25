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

float set_halo_score_local(tree_info *trees, tree_node_info *current_progenitor, int progenitor_mode);
float set_halo_score_local(tree_info *trees, tree_node_info *current_progenitor, int progenitor_mode) {
    if(current_progenitor != NULL) {
        if(progenitor_mode == TREE_PROGENITOR_ORDER_N_PARTICLES)
            return ((float)(current_progenitor->n_particles));
        else if(progenitor_mode == TREE_PROGENITOR_ORDER_N_PARTICLES_INCLUSIVE)
            return ((float)(current_progenitor->n_particles_inclusive));
        else if(progenitor_mode == TREE_PROGENITOR_ORDER_N_PARTICLES_PEAK)
            return ((float)(current_progenitor->n_particles_peak));
        else if(progenitor_mode == TREE_PROGENITOR_ORDER_N_PARTICLES_INCLUSIVE_PEAK)
            return ((float)(current_progenitor->n_particles_inclusive_peak));
        else
            SID_trap_error("Invalid progenitor_mode (%d) in find_treenode_snap_equals_given_recursive().", ERROR_LOGIC, progenitor_mode);
    }
    return (-1.);
}

float find_treenode_snap_equals_given_recursive(tree_info *      trees,
                                                tree_node_info * halo,
                                                int              snap_tree_given,
                                                tree_node_info **treenode_return,
                                                int              progenitor_mode) {
    if(halo != NULL) {
        // Proceed down this progenitor line only if we haven't reached the desired snapshot.
        if(halo->snap_tree > snap_tree_given) {
            tree_node_info *best               = halo;
            float           best_score         = -1.;
            int             best_snap          = -1;
            tree_node_info *current_progenitor = halo->progenitor_first;
            while(current_progenitor != NULL) {
                tree_node_info *prog_best = NULL;
                float prog_score = find_treenode_snap_equals_given_recursive(trees, current_progenitor, snap_tree_given, &prog_best, progenitor_mode);
                int   prog_snap  = prog_best->snap_tree;
                if(prog_score > best_score || (abs(prog_snap - snap_tree_given) < abs(best_snap - snap_tree_given))) {
                    best       = prog_best;
                    best_score = prog_score;
                    best_snap  = prog_snap;
                }
                // Next progenitor
                current_progenitor = current_progenitor->progenitor_next;
            }
            (*treenode_return) = best;
        }
        // This is as far down the line that we want to go.  Pass this halo up
        //    the line as the best choice for this branch.
        else
            (*treenode_return) = halo;

        // Return best progenitor information
        return (set_halo_score_local(trees, (*treenode_return), progenitor_mode));
    }
    (*treenode_return) = NULL;
    return (-1.);
}
