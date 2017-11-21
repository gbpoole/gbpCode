#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_if_extended_type_is_fragmented(tree_horizontal_extended_info *halo) {
    return (SID_CHECK_BITFIELD_SWITCH(halo->type, TREE_CASE_FRAGMENTED_STRAYED) ||
            SID_CHECK_BITFIELD_SWITCH(halo->type, TREE_CASE_FRAGMENTED_NORMAL) ||
            SID_CHECK_BITFIELD_SWITCH(halo->type, TREE_CASE_FRAGMENTED_OTHER));
}
