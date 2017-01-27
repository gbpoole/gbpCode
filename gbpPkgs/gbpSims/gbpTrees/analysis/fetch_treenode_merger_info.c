#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>
#include <gbpTrees_analysis.h>

// We want to measure merger ratios (xi) using the particle counts at the time when
//    the secondary reached its peak size.  Find the halos involved at that time.  Because
//    of skips in the trees, etc. the halos may not be *exactly* contemporaneous, but we can
//    usually tolerate that since the change of *peak* mass accross skips should be small.
int  fetch_treenode_merger_info(tree_info       *trees,
                                tree_node_info **halo_secondary,
                                tree_node_info **halo_primary,   
                                double          *xi,
                                double          *v_rel,
                                double          *sig_v_p,
                                double          *sig_v_s){

   // n.b.: You can choose not to set the primary.  If set to NULL, a check for the first progenitor of the
   //       descendant will be performed.  If it is defined and isn't the same as the secondary, then we will 
   //       proceed with it as the primary.  Otherwise, this will be treated as an invalid merger.
   if((*halo_primary)==NULL && (*halo_secondary)!=NULL){
      tree_node_info *halo_test=(*halo_secondary)->descendant;
      if(halo_test!=NULL){
         halo_test=halo_test->progenitor_primary;
         if(halo_test!=(*halo_secondary))
            (*halo_primary)=halo_test;
      }
   }

   // Find the po secondary and primary
   int flag_success=FALSE;
   if((*halo_secondary)!=NULL && (*halo_primary)!=NULL){
      flag_success=TRUE;
      // For groups, we want to make the comparison immediately before the merger
      if(check_treenode_if_group((*halo_secondary))){
         flag_success=find_treenode_snap_equals_given(trees,
                                                      (*halo_primary),
                                                      (*halo_secondary)->snap_tree,
                                                      halo_primary,
                                                      TREE_PROGENITOR_ORDER_N_PARTICLES);
      }
      // For substructure, we want to make comparisons at the secondary's peak inclusive mass point
      else{
         tree_markers_info *markers_secondary=fetch_treenode_precomputed_markers(trees,(*halo_secondary));
         (*halo_secondary)                   =markers_secondary->peak_mass;
         flag_success=find_treenode_snap_equals_given(trees,
                                                      (*halo_primary),
                                                      (*halo_secondary)->snap_tree,
                                                      halo_primary,
                                                      TREE_PROGENITOR_ORDER_N_PARTICLES_INCLUSIVE_PEAK);
      }
   }

   // Compute merger ratio (xi)
   if((*halo_secondary)!=NULL && (*halo_primary)!=NULL){
      if(xi!=NULL){
         // Fetch peak particle counts for both halos
         int n_p_secondary;
         int n_p_primary;
         if(check_treenode_if_group((*halo_secondary))){
            n_p_secondary=(*halo_secondary)->n_particles;
            n_p_primary  =(*halo_primary)->n_particles;
         }
         else{
            n_p_secondary=(*halo_secondary)->n_particles_inclusive_peak;
            n_p_primary  =(*halo_primary)->n_particles_inclusive_peak;
         }
         (*xi)=(double)n_p_secondary/(double)n_p_primary;
      }

      // Compute relative velocity
      if(v_rel!=NULL){
         double vx_primary  =fetch_treenode_vx(trees,(*halo_primary));
         double vy_primary  =fetch_treenode_vy(trees,(*halo_primary));
         double vz_primary  =fetch_treenode_vz(trees,(*halo_primary));
         double vx_secondary=fetch_treenode_vx(trees,(*halo_secondary));
         double vy_secondary=fetch_treenode_vy(trees,(*halo_secondary));
         double vz_secondary=fetch_treenode_vz(trees,(*halo_secondary));
         (*v_rel)           =sqrt(pow(vx_secondary-vx_primary,2.)+
                                  pow(vy_secondary-vy_primary,2.)+
                                  pow(vz_secondary-vz_primary,2.));
      }

      // Fetch velocity dispersions
      if(sig_v_p!=NULL) (*sig_v_p)=fetch_treenode_sigmav(trees,(*halo_primary));
      if(sig_v_s!=NULL) (*sig_v_s)=fetch_treenode_sigmav(trees,(*halo_secondary));
   }
   else{
      flag_success=FALSE;
      if(xi!=NULL)      (*xi)     =-1;
      if(v_rel!=NULL)   (*v_rel)  = 0.;
      if(sig_v_p!=NULL) (*sig_v_p)= 0.;
      if(sig_v_s!=NULL) (*sig_v_s)= 0.;
   }

   return(flag_success);
}

