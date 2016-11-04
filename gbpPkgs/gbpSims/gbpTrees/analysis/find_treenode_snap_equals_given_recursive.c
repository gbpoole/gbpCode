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

int find_treenode_snap_equals_given_recursive(tree_info *trees,tree_node_info *halo,int snap_tree_given,tree_node_info **treenode_return,int progenitor_mode){
   if((*treenode_return)==NULL && halo->snap_tree==snap_tree_given) 
      (*treenode_return)=halo;
   if((*treenode_return)==NULL){
      // Create sorted list
      int              n_progenitors  =halo->n_progenitors;
      tree_node_info **progenitor_list=(tree_node_info **)SID_malloc(sizeof(tree_node_info *)*n_progenitors);
      int             *score          =(int             *)SID_malloc(sizeof(int             )*n_progenitors);
      size_t          *score_index    =NULL;
      for(int i_progenitor=0;i_progenitor<n_progenitors;i_progenitor++){
         tree_node_info *current_progenitor=NULL;
         if(i_progenitor==0)
            current_progenitor=halo->progenitor_first;
         else
            current_progenitor=(progenitor_list[i_progenitor-1])->progenitor_next;
         progenitor_list[i_progenitor]=current_progenitor;
         if(((trees->mode)&(progenitor_mode))==progenitor_mode) // note that trees->mode has more going on than just progenitor_mode, so need to check it like this
            score[i_progenitor]=(n_progenitors-i_progenitor);
         else if(progenitor_mode==TREE_PROGENITOR_ORDER_N_PARTICLES_PEAK)
            score[i_progenitor]=current_progenitor->n_particles_peak;
         else if(progenitor_mode==TREE_PROGENITOR_ORDER_N_PARTICLES_INCLUSIVE_PEAK)
            score[i_progenitor]=current_progenitor->n_particles_inclusive_peak;
      }
      // Sort progenitors by score
      merge_sort(score,(size_t)n_progenitors,&score_index,SID_FLOAT,SORT_COMPUTE_INDEX,FALSE);
      // Walk trees (note: halos are sorted in ascending order)
      for(int i_progenitor=n_progenitors-1;i_progenitor>=n_progenitors && (*treenode_return)==NULL;i_progenitor--)
         find_treenode_snap_equals_given_recursive(trees,progenitor_list[score_index[i_progenitor]],snap_tree_given,treenode_return,progenitor_mode);
      // Free sorted list
      SID_free(SID_FARG progenitor_list);
      SID_free(SID_FARG score);
      SID_free(SID_FARG score_index);
   }
}

