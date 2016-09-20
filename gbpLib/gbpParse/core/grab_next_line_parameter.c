#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpCommon.h>
#include <gbpSID.h>
#include <gbpParse_core.h>
int grab_next_line_parameter(FILE *fp,char **line, size_t *n){
   int rval=grab_next_line(fp,line,n);
   while(!feof(fp) && !check_parameter(*line))
      rval=grab_next_line(fp,line,n);
   return(rval);
}
