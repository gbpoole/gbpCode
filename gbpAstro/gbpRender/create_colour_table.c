#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gbpLib.h>
#include <gbpRender.h>

void create_colour_table(const char   *colourmapselect_in,
                         int          *n_colours_in,
                         int        ***rgb){
   char  filename_in[MAX_FILENAME_LENGTH];
   char  colourmapselect[MAX_FILENAME_LENGTH];
   FILE *fp_in=NULL;
   int   cmap_len=strlen(colourmapselect_in);
   int   flag_invert=FALSE;
   strcpy(colourmapselect,colourmapselect_in);
   if(!strcmp(&(colourmapselect[cmap_len-2]),"_r")){
      flag_invert=TRUE;
      colourmapselect[cmap_len-2]='\0';
   }
   sprintf(filename_in,"%s/gbpCode_cmap_%s.txt",GBP_DATA_DIR,colourmapselect);
   if((fp_in=fopen(filename_in,"r"))==NULL){
      SID_trap_error("Could not open colour table {%s}.",ERROR_LOGIC,colourmapselect);
   }
   int n_colours      =count_lines_data(fp_in);
   (*n_colours_in)=n_colours;
   char  *line       =NULL;
   size_t line_length=0;
   (*rgb)   =(int **)SID_malloc(sizeof(int *)*3);
   (*rgb)[0]=(int  *)SID_malloc(sizeof(int)*n_colours);
   (*rgb)[1]=(int  *)SID_malloc(sizeof(int)*n_colours);
   (*rgb)[2]=(int  *)SID_malloc(sizeof(int)*n_colours);
   for(int i_read=0;i_read<n_colours;i_read++){
      double r_i;
      double g_i;
      double b_i;
      grab_next_line_data(fp_in,&line,&line_length); 
      grab_double(line,2,&r_i);
      grab_double(line,3,&g_i);
      grab_double(line,4,&b_i);
      (*rgb)[0][i_read]=MAX(0,MIN(255,(int)(255.*r_i)));
      (*rgb)[1][i_read]=MAX(0,MIN(255,(int)(255.*g_i)));
      (*rgb)[2][i_read]=MAX(0,MIN(255,(int)(255.*b_i)));
   }
   SID_free(SID_FARG line);
   fclose(fp_in);

   if(flag_invert){
      int *rgb_temp=(int *)SID_malloc(sizeof(int)*(n_colours));
      for(int j=0;j<n_colours;j++)
        rgb_temp[j]=(*rgb)[0][n_colours-j-1];
      for(int j=0;j<n_colours;j++)
        (*rgb)[0][j]=rgb_temp[j];
      for(int j=0;j<n_colours;j++)
        rgb_temp[j]=(*rgb)[1][n_colours-j-1];
      for(int j=0;j<n_colours;j++)
        (*rgb)[1][j]=rgb_temp[j];
      for(int j=0;j<n_colours;j++)
        rgb_temp[j]=(*rgb)[2][n_colours-j-1];
      for(int j=0;j<n_colours;j++)
        (*rgb)[2][j]=rgb_temp[j];
      SID_free(SID_FARG rgb_temp);
   }

}
