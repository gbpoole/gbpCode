#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpCommon.h>
#include <gbpParse_core.h>

int check_space(char *line){
   return(!strncmp(line," ", 1) ||
          !strncmp(line,"\t",1) ||
          !strncmp(line,"\0",1) ||
          !strncmp(line,"\n",1)); // true if one of these, false if none
}
