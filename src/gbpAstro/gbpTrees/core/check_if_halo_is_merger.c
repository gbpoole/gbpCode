#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpTrees_build.h>

int check_if_halo_is_merger(int type) {
    return (SID_CHECK_BITFIELD_SWITCH(type, TREE_CASE_MERGER));
}
