#include <gbpLib.h>
#include <gbpTrees_build.h>

void init_trees_horizontal_snapshot(tree_horizontal_info *halos,
                                    int                   i_read,
                                    int                   i_file,
                                    int                   n_groups,
                                    int                   n_groups_max,
                                    int                   n_subgroups,
                                    int                   n_subgroups_max,
                                    int                  *n_subgroups_group,
                                    int                   flag_match_subgroups){
   int n_halos_max;
   int n_halos;
   if(flag_match_subgroups==MATCH_GROUPS){
      n_halos_max=n_groups_max;
      n_halos    =n_groups;
   }
   else if(flag_match_subgroups==MATCH_SUBGROUPS){
      n_halos_max=n_subgroups_max;
      n_halos    =n_subgroups;
   }
   else
      SID_trap_error("flag_match_subgroups not set correctly (%d) in init_trees_horizontal_snapshot().",ERROR_LOGIC,flag_match_subgroups);
   for(int i_halo=0,i_group=0,i_subgroup=0,j_subgroup=0;i_halo<n_halos_max;i_halo++,j_subgroup++){
      // Set the appropriate counters
      if(flag_match_subgroups==MATCH_SUBGROUPS){
         i_subgroup=i_halo;
         if(i_group<n_groups){
            while(j_subgroup==n_subgroups_group[i_group] && i_group<(n_groups-1)){
               i_group++;
               j_subgroup=0;
            }
            if(j_subgroup==n_subgroups_group[i_group]){
               i_group++;
               j_subgroup=0;
            }
         }
      }
      halos[i_halo].file                          =  i_file;
      halos[i_halo].snap                          =  i_read;
      halos[i_halo].index                         =  (size_t)i_halo;
      halos[i_halo].n_back_matches                =    0;
      halos[i_halo].descendant.halo               = NULL;
      halos[i_halo].descendant.score              =   0.;
      halos[i_halo].first_progenitor.halo         = NULL;
      halos[i_halo].first_progenitor.score        =   0.;
      halos[i_halo].first_progenitor.flag_two_way =FALSE;
      halos[i_halo].last_progenitor.halo          = NULL;
      halos[i_halo].last_progenitor.score         =   0.;
      halos[i_halo].last_progenitor.flag_two_way  =FALSE;
      halos[i_halo].next_progenitor.halo          = NULL;
      halos[i_halo].next_progenitor.score         =   0.;
      halos[i_halo].next_progenitor.flag_two_way  =FALSE;
      halos[i_halo].forematch_first.halo          = NULL;
      halos[i_halo].forematch_first.score         =   0.;
      halos[i_halo].forematch_first.flag_two_way  =FALSE;
      halos[i_halo].forematch_default.halo        = NULL;
      halos[i_halo].forematch_default.score       =   0.;
      halos[i_halo].forematch_default.flag_two_way=FALSE;
      halos[i_halo].forematch_best.halo           = NULL;
      halos[i_halo].forematch_best.score          =   0.;
      halos[i_halo].forematch_best.flag_two_way   =FALSE;
      halos[i_halo].bridge_backmatch.halo         = NULL;
      halos[i_halo].bridge_backmatch.score        =   0.;
      halos[i_halo].bridge_backmatch.flag_two_way =FALSE;
      SID_free(SID_FARG halos[i_halo].back_matches);
      if(i_halo<n_halos){
         halos[i_halo].type=TREE_CASE_UNPROCESSED|TREE_CASE_MAIN_PROGENITOR|TREE_CASE_NO_PROGENITORS;
         if(flag_match_subgroups==MATCH_SUBGROUPS && j_subgroup==0)
            halos[i_halo].type|=TREE_CASE_MOST_MASSIVE;
      }
      else
         halos[i_halo].type=TREE_CASE_INVALID;
      halos[i_halo].id                =-1;
      halos[i_halo].main_progenitor_id=-1;
      halos[i_halo].tree_id           =-1;
      halos[i_halo].n_particles       = 0;
      halos[i_halo].n_particles_parent= 0;
      halos[i_halo].n_particles_largest_descendant= 0;
      halos[i_halo].n_progenitors                 = 0;
   }
}

