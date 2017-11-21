#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_if_type_is_fragmented(int type) {
    return (SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_STRAYED) ||
            SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_NORMAL) ||
            SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_FRAGMENTED_OTHER));
}
