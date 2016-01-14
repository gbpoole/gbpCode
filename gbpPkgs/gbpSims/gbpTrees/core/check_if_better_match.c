#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_if_better_match(tree_horizontal_info *target_halo,
                          match_info           *old_match,
                          match_info           *new_match){
   int flag_valid=TRUE;
   if(old_match!=NULL){
      if(old_match->halo!=NULL){
         if(new_match==NULL)
            flag_valid=FALSE;
         else if(new_match->halo==NULL)
            flag_valid=FALSE;
         else{
            int file_offset_new=(target_halo->file)-(new_match->halo->file);
            int file_offset_old=(target_halo->file)-(old_match->halo->file);
            // Since we might use this code to check back matches,
            //    correct the sence of the offset
            if(file_offset_new<0){
               file_offset_new=-file_offset_new;
               file_offset_old=-file_offset_old;
            }
            // Sanity check
            if(file_offset_new<=0 || file_offset_old<=0)
               SID_trap_error("There is something odd about the file offsets (%d->%d and %d->%d) in check_if_better_match().",ERROR_LOGIC,new_match->halo->file,target_halo->file,old_match->halo->file,target_halo->file);
            // The best match is always the most immediate
            if(file_offset_new>file_offset_old)
               flag_valid=FALSE;
            else if(file_offset_new==file_offset_old){
               // If equally immediate, give first preference to 2-way matches ...
               if(old_match->flag_two_way)
                  flag_valid=FALSE;
               // ... otherwise, give secondary preference to the halo with the best metrics.
               //     Unfortunately, the best choice of metric(s) is not perfectly obvious:
               //    ... using match score for this decision introduces the problem
               //        of sometimes selecting a small substructure that sinks quickly to (or
               //        passes transiently through) the middle of a larger halo.  This 
               //        problem is reduced by giving preference to 2-way matches (above).
               //    ... using n_particles for this decision can be compromised
               //        by ambiguities introduced by the large halo size fluctuations
               //        introduced by transient exchanges of outer halo particles
               //else if((old_match->halo->n_particles)>(new_match->halo->n_particles))
               else if((old_match->score)>(new_match->score))
                  flag_valid=FALSE;
            }
         }
      }
   }
   return(flag_valid);
}
