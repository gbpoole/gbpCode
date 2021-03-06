#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

void compute_substructure_order_recursive(tree_node_info *parent, int *score_parent, int mode) {
    if(parent->n_substructures > 1) {
        tree_node_info **node_list;
        int *            score;
        node_list = (tree_node_info **)SID_malloc(sizeof(tree_node_info *) * parent->n_substructures);
        score     = (int *)SID_malloc(sizeof(int) * parent->n_substructures);

        // Loop over each subgroup (recursively)
        tree_node_info *current_subgroup;
        size_t          n_halos = 0;
        current_subgroup        = parent->substructure_first;
        while(current_subgroup != NULL) {
            node_list[n_halos] = current_subgroup;                                           // Make node  list
            compute_substructure_order_recursive(current_subgroup, &(score[n_halos]), mode); // Make score list
            n_halos++;
            current_subgroup = current_subgroup->substructure_next;
        }
        if(n_halos != parent->n_substructures)
            SID_exit_error("Invalid halo count (%d!=%d) in compute_substructure_order_score_recursive().",
                           SID_ERROR_LOGIC, n_halos, parent->n_substructures);

        // Sort the resulting scores (ascending!) ...
        size_t *score_index = NULL;
        merge_sort(score, n_halos, &score_index, SID_INT, SORT_COMPUTE_INDEX, SORT_COMPUTE_NOT_INPLACE);

        // Reorder the pointers in *decending* order
        int             i_halo;
        tree_node_info *new_first;
        tree_node_info *new_last;
        new_first = node_list[score_index[n_halos - 1]];
        new_last  = node_list[score_index[0]];
        for(i_halo = 0; i_halo < n_halos; i_halo++) {
            if(i_halo == 0)
                node_list[score_index[i_halo]]->substructure_next = NULL;
            else
                node_list[score_index[i_halo]]->substructure_next = node_list[score_index[i_halo - 1]]; // descending
        }
        parent->substructure_first = new_first;
        parent->substructure_last  = new_last;

        // Clean-up
        SID_free(SID_FARG score_index);
        SID_free(SID_FARG score);
        SID_free(SID_FARG node_list);
    }
    // This should never be needed but just in case
    else if(parent->substructure_first != NULL)
        compute_substructure_order_recursive(parent->substructure_first, NULL, mode);

    // Pass order scores up the heirarchy
    if(SID_CHECK_BITFIELD_SWITCH(mode, TREE_SUBSTRUCTURE_ORDER_DEFAULT)) {
        if(score_parent != NULL)
            (*score_parent) = parent->n_particles;
    } else
        SID_exit_error("Invalid mode (%d) in assign_substructure_order_recursive().", SID_ERROR_LOGIC, mode);
}
