#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gbpLib.h>
#include <gbpSPH.h>

// In the future ... receive a list of variables to make this adaptive

void write_gadget_bin(char       *filename_out,
	                  plist_info *plist){

  SID_log("Writing .bin file {%s}...",SID_LOG_OPEN,filename_out);

  float h_Hubble=(float)(((double *)ADaPS_fetch(plist->data,"h_Hubble"))[0]);

  // Note that this code assumes that all particle IDs are present and they are
  //   ordered by species.  Running will abort if this is not the case.  Also, the
  //   I/O as written is VERY inefficient.  Sorry ... this was a rush-job.
  size_t i_p=0;
  size_t j_p=0;
  for(int i_rank=0;i_rank<SID.n_proc;i_rank++){
     if(i_rank==SID.My_rank){
        FILE *fp_out=NULL;
        if(i_rank==0)
           fp_out=fopen(filename_out,"w");
        else
           fp_out=fopen(filename_out,"a");
        for(int i_species=0;i_species<N_GADGET_TYPE;i_species++){
            if(ADaPS_exist(plist->data,"n_all_%s",plist->species[i_species])){
               size_t n_p_all=((size_t *)ADaPS_fetch(plist->data,"n_all_%s",plist->species[i_species]))[0];
               size_t n_p    =((size_t *)ADaPS_fetch(plist->data,"n_%s",    plist->species[i_species]))[0];
               if(n_p>0){
                  GBPREAL *x =(GBPREAL *)ADaPS_fetch(plist->data,"x_%s", plist->species[i_species]);
                  GBPREAL *y =(GBPREAL *)ADaPS_fetch(plist->data,"y_%s", plist->species[i_species]);
                  GBPREAL *z =(GBPREAL *)ADaPS_fetch(plist->data,"z_%s", plist->species[i_species]);
                  size_t  *id=(size_t  *)ADaPS_fetch(plist->data,"id_%s",plist->species[i_species]);
                  size_t  *id_index=NULL;
                  merge_sort(id,n_p,&id_index,SID_SIZE_T,SORT_COMPUTE_INDEX,FALSE);
                  for(int i_p=0;i_p<n_p_all;i_p++){
                      int flag=FALSE;
                      if(j_p<n_p){
                         size_t index=id_index[j_p];
                         if(id[index]==i_p){
                            // Open file
                            float x_i=h_Hubble*(float)(x[index])/M_PER_MPC;
                            float y_i=h_Hubble*(float)(y[index])/M_PER_MPC;
                            float z_i=h_Hubble*(float)(z[index])/M_PER_MPC;
                            fwrite(&x_i,sizeof(float),1,fp_out);
                            fwrite(&y_i,sizeof(float),1,fp_out);
                            fwrite(&z_i,sizeof(float),1,fp_out);
                            //fprintf(fp_out,"%3d %le %le %le\n",id[index],x_i,y_i,z_i);
                            j_p++;
                            flag=TRUE;
                         }
                      }
                      int n_write=0;
                      SID_Allreduce(&flag,&n_write,1,SID_INT,SID_SUM,SID.COMM_WORLD);
                      if(n_write!=1)
                          SID_trap_error("Particle %d was not written correctly.",ERROR_LOGIC,i_p);
                  }
               }
            }
        } // i_species
        fclose(fp_out);
     } // if i_rank
     SID_Barrier(SID.COMM_WORLD);
  }
  SID_log("Done.",SID_LOG_CLOSE);
}

