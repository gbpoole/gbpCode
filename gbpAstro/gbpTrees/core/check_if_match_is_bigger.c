#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_if_match_is_bigger(tree_horizontal_info *target_halo, match_info *old_match, match_info *new_match) {
    int flag_valid = GBP_TRUE;
    if(old_match != NULL) {
        if(old_match->halo != NULL) {
            if(new_match == NULL)
                flag_valid = GBP_FALSE;
            else if(new_match->halo == NULL)
                flag_valid = GBP_FALSE;
            else if(old_match->halo->n_particles > new_match->halo->n_particles)
                flag_valid = GBP_FALSE;
        }
    }
    return (flag_valid);
}
