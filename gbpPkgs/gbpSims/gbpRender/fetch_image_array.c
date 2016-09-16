#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void fetch_image_array(image_info **image,int i_depth,double **values){
   if(image!=NULL)
      (*values)=image[i_depth]->values;
   else
      (*values)=NULL;
}
