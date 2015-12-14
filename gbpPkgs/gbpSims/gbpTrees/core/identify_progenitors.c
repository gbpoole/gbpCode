#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpTrees_build.h>

void identify_progenitors(tree_horizontal_info **halos,
                          tree_horizontal_info  *halos_i,
                          int   **n_subgroups_group,
                          int     n_halos_i,
                          int    *match_id,
                          float  *match_score,
                          size_t *match_index,
                          char   *match_flag_two_way,
                          int    *n_particles,
                          int     i_file,
                          int     i_read,
                          int     i_read_start,
                          int     i_read_stop,
                          int     i_read_step,
                          int     n_search,
                          int     n_wrap,
                          int     n_halos_max,
                          int     n_files,
                          int     flag_fix_bridges,
                          int    *max_id,
                          int    *n_halos_1_matches,
                          int    *n_halos_2_matches,
                          char   *filename_root_matches,
                          char   *group_text_prefix,
                          int     flag_match_subgroups){
   SID_log("Identifying progenitors...",SID_LOG_OPEN|SID_LOG_TIMER);
   int j_file_1;
   int j_file_2;
   int j_read_1;
   int j_read_2;
   int i_search;
   int i_halo;
   for(j_file_1  =i_file,
         j_file_2=i_file+1,
         j_read_1=i_read,
         j_read_2=i_read+i_read_step,
         i_search=0;
       j_read_2<=i_read_stop && i_search<n_search;
       j_file_2++,
         j_read_2+=i_read_step,
         i_search++){

      // Read forward-matching
      read_matches(filename_root_matches,
                   j_read_1,j_read_2,n_halos_max,
                   flag_match_subgroups,
                   n_halos_1_matches,
                   n_halos_2_matches,
                   n_particles,
                   NULL,
                   n_subgroups_group[j_file_1%n_wrap],
                   n_subgroups_group[j_file_2%n_wrap],
                   match_id,
                   match_score,
                   match_index,
                   match_flag_two_way,
                   F_GOODNESS_OF_MATCH);

      // Store halo sizes.  If flag_fix_bridges is on, then this was already done before.
      if(!flag_fix_bridges && i_search==0){
         for(i_halo=0;i_halo<(*n_halos_1_matches);i_halo++)
            halos[i_file%n_wrap][i_halo].n_particles=n_particles[i_halo];
      }

      // Perform matching for all the halos in i_file_1.  This loop first tries to identify a
      //   first/default match for each halo in this snapshot, ultimately dealing with all simple 
      //   matches and dropped halos (finalized later).  Matches to emerged halos (either coming 
      //   from the default match, or to emerged candidates of bridged halos in it's descendant 
      //   line) get dealt with subsequently.
      tree_horizontal_info *halos_i=halos[j_file_1%n_wrap];
      tree_horizontal_info *halos_j=halos[j_file_2%n_wrap];
      for(i_halo=0;i_halo<(*n_halos_1_matches);i_halo++){
         tree_horizontal_info *forematch_i=halos_i[i_halo].forematch_default.halo;
         // For bridged halos, initialize the first match to be the largest and most immediate back match to it.
         //   This is the prefered choice (vs the back match with the best match score for example)
         //   because it minimizes errors that get made later when estimating peak halo sizes, 
         //   particularly when bridged halos are satellites of a system and the DOMINANT halo
         //   mechanism can not be used to prevent large mass excursions.
         if(check_mode_for_flag(halos_i[i_halo].type,TREE_CASE_BRIDGED)){
            if(!check_mode_for_flag(halos_i[i_halo].type,TREE_CASE_UNPROCESSED)){
               int i_best=0; // should this be changed if another back match is more immediate?
               halos_i[i_halo].forematch_first.halo          =halos_i[i_halo].back_matches[i_best].halo;
               halos_i[i_halo].forematch_first.score         =halos_i[i_halo].back_matches[i_best].score;
               halos_i[i_halo].forematch_first.flag_two_way  =halos_i[i_halo].back_matches[i_best].flag_two_way;
               memcpy(&(halos_i[i_halo].forematch_default),&(halos_i[i_halo].forematch_first.halo),sizeof(match_info));
               halos_i[i_halo].type|=  TREE_CASE_SET_BY_BACKMATCH; // later, turn this off if default match != first match is used
               halos_i[i_halo].type&=(~TREE_CASE_UNPROCESSED); 
            }
         }
         // ... else, if this halo hasn't been forward-matched to anything yet ...
         else if(forematch_i==NULL){
            int my_descendant_index;
            my_descendant_index=match_id[i_halo];
            // If this halo has been matched to something in i_file_2 ...
            if(my_descendant_index>=0){
               if(my_descendant_index<(*n_halos_2_matches)){
                  halos_i[i_halo].forematch_first.halo          =&(halos_j[my_descendant_index]);
                  halos_i[i_halo].forematch_first.score         =match_score[i_halo];
                  halos_i[i_halo].forematch_first.flag_two_way  =match_flag_two_way[i_halo];
                  memcpy(&(halos_i[i_halo].forematch_default),&(halos_i[i_halo].forematch_first.halo),sizeof(match_info));
               }
               else
                  SID_log_warning("descendant ID out of bounds (ie. %d>%d) in snapshot %03d -> snapshot %03d %sgroup matching for i_halo=%d.",
                                  SID_WARNING_DEFAULT,my_descendant_index,(*n_halos_2_matches)-1,j_read_1,j_read_2,group_text_prefix,i_halo);
            }
         }
         // Once a halo has passed one of the previous checks, after having formed 
         //    a first match, scan the candidate emerged halos in the descendant 
         //    line of the current default match. We don't have to address
         //    bridged halos matched above in this iteration of the loop
         //    since all emerged halos will be from the match files we will
         //    subsequently read (hence the 'else' is ok).  The check on 
         //    TREE_CASE_UNPROCESSED will fail if/once we have scanned all
         //    the possible emerged candidates (and emerged candidates of 
         //    good emerged matches, etc) for this halo.
         else if(check_mode_for_flag(halos_i[i_halo].type,TREE_CASE_UNPROCESSED)){
            int n_unchecked=0;
            int flag_unchanged=TRUE;
            while(forematch_i!=NULL && flag_unchanged){
               if(check_mode_for_flag(forematch_i->type,TREE_CASE_BRIDGED)){
                  back_match_info *back_matches  =halos[(forematch_i->file)%n_wrap][forematch_i->index].back_matches;
                  int              n_back_matches=halos[(forematch_i->file)%n_wrap][forematch_i->index].n_back_matches;
                  // Loop over all the candidate emerged halos identified with this
                  //   halo in the descendant line of the current default match
                  for(int k_halo=0;k_halo<n_back_matches && flag_unchanged;k_halo++){
                     tree_horizontal_info *current_back_match=back_matches[k_halo].halo;
                     // If this back match is in the snapshot we are currently checking
                     if(current_back_match->file==j_file_2 && current_back_match->index==match_id[i_halo]){
                        // If this is a good match to an emerged halo, then set a new default.  Do not remove the UNPROCESSED flag
                        //    though, because we still want to check emerged halo candidates in the descendant line of the new match
                        if(check_validity_of_emerged_match(&(halos_i[i_halo]),&(back_matches[k_halo]),match_flag_two_way[i_halo],n_search)){
                           halos_i[i_halo].forematch_default.halo        =&(halos[j_file_2%n_wrap][current_back_match->index]);
                           halos_i[i_halo].forematch_default.score       =match_score[i_halo];
                           halos_i[i_halo].forematch_default.flag_two_way=match_flag_two_way[i_halo];
                           flag_unchanged=FALSE;
                        }
                     }
                     // Keep count of back matches still to be checked in later snapshots
                     else if((back_matches[k_halo].halo->file)>j_file_2)
                        n_unchecked++;
                  }
               }
               forematch_i=forematch_i->descendant.halo;
               if(forematch_i!=NULL){
                  if((forematch_i->file-i_file)>=n_search || forematch_i->snap==i_read_stop)
                     forematch_i=NULL;
               }
            }
            // Stop processing this halo if there are no more
            //    emerged candaidates to check in later snapshots
            if(n_unchecked==0 && flag_unchanged)
               halos_i[i_halo].type&=(~TREE_CASE_UNPROCESSED); 
         }
      }
   }

   // Now that all first and default forematch pointers have been set,
   //    determine the best match to any halos matched to from this snapshot,
   //    that don't have a progenitor yet.  This is done to make sure
   //    that every bridged halo matched to gets a main progenitor.  
   //    Importantly, this must be in agreement with the choices that have
   //    already been made regarding the main progenitor exiting a bridged halo.  
   //    To acheive this in a robust way, we will exclude here (if possible)
   //    any halos which have default forematch pointers that arent't their 
   //    first forematch pointer (reflecting a halo that has been successfully 
   //    matched in later snapshots to an emerged candidate).  Such a halo
   //    would not have been chosen as the main progenitor exiting the bridged
   //    system, since it and all its descendants would have been excluded
   //    when candidate emerged halo lists were constructed.  We need to wait
   //    for both the first and default pointers to be properly set to do this,
   //    so we need to do this now as a seperate and subsequent loop.  When
   //    matches are finalized, these best pointers will be the first choice
   //    for constructing the trees.
   // Loop over all halos ...
   for(i_halo=0;i_halo<(*n_halos_1_matches);i_halo++){
      tree_horizontal_info *halo_i=&(halos_i[i_halo]);
      tree_horizontal_info *halo_j=halo_i->forematch_first.halo;
      // ... if this halo has a first match ...
      if(halo_j!=NULL){
         // ... if the halo matched to does not have any progenitors yet ...
         if(halo_j->n_progenitors==0){
            match_info forematch_new;
            forematch_new.halo        =halo_i;
            forematch_new.score       =halo_i->forematch_first.score;
            forematch_new.flag_two_way=halo_i->forematch_first.flag_two_way;
            // ... if this is the first match, choose it by default ...
            if(halo_j->forematch_best.halo==NULL)
               memcpy(&(halo_j->forematch_best),&forematch_new,sizeof(match_info));
            // ... else, if this is a subsequent match, decide if it is better ...
            else{
               // ... first, do the important check for subsequent emerged halo matches ...
               tree_horizontal_info *halo_k=halo_j->forematch_best.halo;
               int flag_matched_to_emerged_new=(!((halo_i->forematch_first.halo)==(halo_i->forematch_default.halo)));
               int flag_matched_to_emerged_old=(!((halo_k->forematch_first.halo)==(halo_k->forematch_default.halo)));
               if(!flag_matched_to_emerged_new && flag_matched_to_emerged_old)
                  memcpy(&(halo_j->forematch_best),&forematch_new,sizeof(match_info));
               // ... else do a check on validity if neither-or-both the new and previous best matches have a match to an emerged candidate
               else if((flag_matched_to_emerged_new==flag_matched_to_emerged_old) && 
                       check_validity_of_main_progenitor(halo_j,
                                                         &(halo_j->forematch_best),
                                                         &forematch_new)){
                  memcpy(&(halo_j->forematch_best),&forematch_new,sizeof(match_info));
               }
            }
         }
      }
   }
   SID_log("Done.",SID_LOG_CLOSE);
}
