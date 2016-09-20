#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpCommon.h>
#include <gbpParse_core.h>

int check_parameter(char *line){
   int  line_length  =strlen(line);
   int  r_val        =FALSE;
   int  flag_continue=TRUE;
   if(count_words(line)>0){
      for(int i_line=0;i_line<line_length && flag_continue;i_line++){
         if(!check_space(line+i_line)){
            flag_continue=FALSE;
            if(!strncmp(line+i_line,GBPPARSE_PARAMETER_CHARACTER,1))
               r_val=TRUE;
         }
      }
   }
   else
      r_val=TRUE;
   return(r_val);
}
