#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_validity_of_emerged_match(tree_horizontal_info *halo_i, match_info *back_match, int n_search) {
    // Perform some checks to see if we want to make a match to this candidate emerged halo
    int flag_valid = GBP_TRUE;

    // 1) The TREE_CASE_EMERGED_CANDIDATE flag must be set for the back-matched halo.
    //    This isn't the case if this back match is in the main progenitor line of it's bridge.
    flag_valid = SID_CHECK_BITFIELD_SWITCH(back_match->halo->type, TREE_CASE_EMERGED_CANDIDATE);

    if(flag_valid) {
        // 2) The candidate must not have a progenitor yet.  This ensures that an emerged
        //    halo's progenitor is as immediate as possible and prevents mergers before
        //    a halo gets lost in a bridge from being incorrectly converted into mergers
        //    with the emerged candidate (for example).
        if(back_match->halo->first_progenitor.halo != NULL)
            flag_valid = GBP_FALSE;
        if(flag_valid) {
            // 3) Because we may be recursively finding emerged halos, make sure we haven't
            //    exceeded the search interval.  If so, invalidate the match.
            int total_offset = back_match->halo->file - halo_i->file;
            if(total_offset <= 0)
                SID_exit_error("Invalid emerged halo snapshot offset (ie. %d<=0).", SID_ERROR_LOGIC, total_offset);
            if(total_offset > n_search)
                flag_valid = GBP_FALSE;
        }
    }
    return (flag_valid);
}
