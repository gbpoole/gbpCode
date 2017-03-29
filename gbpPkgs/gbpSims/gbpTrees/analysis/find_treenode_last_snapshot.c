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

int find_treenode_last_snapshot(tree_info *trees,tree_node_info *halo,tree_node_info **branch_root){
   // n.b.: Because halo ids can change in the middle of progenitor lines, we want to scan until
   //       there is no descendant or the current descenant is a merger.  That is the end of the
   //       line for a halo.
   //int r_val=FALSE;
   //(*branch_root)=halo;
   //while((*branch_root)!=NULL && !check_treenode_if_merger((*branch_root))){
   //   if((*branch_root)->descendant==NULL) break;
   //   (*branch_root)=(*branch_root)->descendant;
   //   r_val=TRUE;
   //}
   //return(r_val);
   (*branch_root)=halo;
   while(check_treenode_if_main_progenitor((*branch_root)))
      (*branch_root)=(*branch_root)->descendant;
   return(TRUE);
}

