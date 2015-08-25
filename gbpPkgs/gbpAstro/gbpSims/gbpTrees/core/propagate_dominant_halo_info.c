#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

void propagate_dominant_halo_info(tree_horizontal_extended_info **groups,   int *n_groups,
                                  tree_horizontal_extended_info **subgroups,int *n_subgroups,
                                  int        **n_subgroups_group,
                                  int          i_read, // tree snapshot index
                                  int          j_read, // actual snapshot index
                                  int          l_read,
                                  int          i_read_step,
                                  int          n_wrap){
   SID_log("Propagating dominant halo info for snapshot #%03d...",SID_LOG_OPEN,j_read);
   // Process groups
   int i_group;
   int i_subgroup;
   int j_subgroup;
   int flag_returned;
   for(i_group=0,i_subgroup=0;i_group<n_groups[l_read];i_group++){
      // Set dominant halo info
      tree_horizontal_extended_info *this_group=&(groups[i_read%n_wrap][i_group]);
      set_extended_dominant_info(groups,this_group,FALSE,i_read,i_group,n_wrap);

      // Check if there are any subgroups in the group that have a dominant flag already
      int flag_parent_has_dominant=FALSE;
      int k_subgroup              =i_subgroup;
      for(j_subgroup=0;j_subgroup<n_subgroups_group[i_read%n_wrap][i_group];k_subgroup++,j_subgroup++)
         flag_parent_has_dominant|=check_mode_for_flag(subgroups[i_read%n_wrap][k_subgroup].type,TREE_CASE_DOMINANT);

      // Process subgroups
      for(j_subgroup=0;j_subgroup<n_subgroups_group[i_read%n_wrap][i_group];i_subgroup++,j_subgroup++){
         // Set dominant halo flags
         tree_horizontal_extended_info *this_subgroup=&(subgroups[i_read%n_wrap][i_subgroup]);
         set_extended_dominant_info(subgroups,this_subgroup,flag_parent_has_dominant,i_read,i_subgroup,n_wrap);
      }
   }
   SID_log("Done.",SID_LOG_CLOSE);
}

