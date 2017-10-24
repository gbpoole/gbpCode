#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpSID.h>
#include <gbpParse_core.h>

int grab_word(char *line,int n,char *return_value){
   char *word=NULL;
   scan_to_nth_word(line,n,&word);
   // Find word length
   size_t length=(size_t)strlen(word);
   size_t count =0;
   for(size_t i_line=0;i_line<length;i_line++){
      if(check_space(word+i_line))
         break;
      else
         count++;
   }
   strncpy(return_value,word,count);
   return_value[count]='\0';
   return(ERROR_NONE);
}
