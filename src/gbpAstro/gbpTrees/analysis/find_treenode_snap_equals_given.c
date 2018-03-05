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

int find_treenode_snap_equals_given(tree_info *      trees,
                                    tree_node_info * halo,
                                    int              snap_tree_given,
                                    tree_node_info **treenode_return,
                                    int              progenitor_mode) {
    (*treenode_return) = halo;
    if(halo != NULL) {
        if(halo->snap_tree < snap_tree_given) {
            int flag_continue = GBP_TRUE;
            while((*treenode_return) != NULL && flag_continue) {
                tree_node_info *descendant = (*treenode_return)->descendant;
                if(descendant != NULL) {
                    if(descendant->snap_tree <= snap_tree_given)
                        (*treenode_return) = descendant;
                    else
                        flag_continue = GBP_FALSE;
                } else
                    flag_continue = GBP_FALSE;
            }
        } else {
            int flag_continue = GBP_TRUE;
            while((*treenode_return) != NULL && flag_continue) {
                tree_node_info *progenitor = (*treenode_return)->progenitor_first;
                if(progenitor != NULL) {
                    if(progenitor->snap_tree >= snap_tree_given)
                        (*treenode_return) = progenitor;
                    else
                        flag_continue = GBP_FALSE;
                } else
                    flag_continue = GBP_FALSE;
            }
        }
        // else if(halo->snap_tree>snap_tree_given)
        //   find_treenode_snap_equals_given_recursive(trees,halo,snap_tree_given,treenode_return,progenitor_mode);
    }
    return ((*treenode_return) != NULL);
}
