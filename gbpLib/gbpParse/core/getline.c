#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpCommon.h>
#include <gbpParse_core.h>
int getline(char **string,size_t *n,FILE *fp){
   size_t len  = 0;
   size_t size = (*n);
   if(size<=0 || (*string)==NULL){
      size   =0;  
      *string=NULL;
   }
   do {
      size     +=BUFSIZ;                        // BUFSIZ is defined as "the optimal read size for this platform"
      (*string) =(char *)realloc(*string,size); // realloc(NULL,n) is the same as malloc(n)
      fgets((*string)+len,BUFSIZ,fp);
      len=strlen(*string); 
   } while (!feof(fp) && (*string)[len]!='\n');
   (*n)=size;
   return((int)len);
}

