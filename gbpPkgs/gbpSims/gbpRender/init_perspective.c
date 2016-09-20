#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void init_perspective(perspective_info **perspective){
  (*perspective)=(perspective_info *)SID_calloc(sizeof(perspective_info));
  (*perspective)->p_o[0]       = 0.;
  (*perspective)->p_o[1]       = 0.;
  (*perspective)->p_o[2]       = 0.;
  (*perspective)->theta        = 0.;
  (*perspective)->zeta         = 0.;
  (*perspective)->FOV          = 0.;
  (*perspective)->focus_shift_x= 0.;
  (*perspective)->focus_shift_y= 0.;
  (*perspective)->radius       = 0.;
  (*perspective)->phi          = 1.;
  (*perspective)->time         = 0.;
}

