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
#include <assert.h>

int check_merger_type_local(tree_info *trees, tree_node_info *halo);
int check_merger_type_local(tree_info *trees, tree_node_info *halo) {
    if(check_treenode_if_merger(halo))
        return (1);
    else if(check_treenode_if_satellite(halo) && fetch_treenode_snap_tree(trees, halo) == (trees->n_snaps - 1))
        return (2);
    else
        return (0);
}

void compute_trees_analysis_mergers(tree_info *trees, char *filename_out_root_in, int i_type, double logM_min, double dlogM, int n_logM) {
    // Loop over both halo types
    tree_node_info **neighbour_list_start = NULL;
    char             group_prefix[8];
    if(i_type == 0) {
        sprintf(group_prefix, "");
        neighbour_list_start = trees->first_neighbour_groups;
    } else {
        sprintf(group_prefix, "sub");
        neighbour_list_start = trees->first_neighbour_subgroups;
    }

    char filename_out_root[SID_MAX_FILENAME_LENGTH];
    sprintf(filename_out_root, "%s_%sgroup", filename_out_root_in, group_prefix);

    SID_log("Performing %sgroup merger analysis...", SID_LOG_OPEN | SID_LOG_TIMER, group_prefix);

    // Count the number of mergers
    SID_log("Counting mergers...", SID_LOG_OPEN | SID_LOG_TIMER);
    int n_mergers = 0;
    for(int i_snap = 0; i_snap < trees->n_snaps; i_snap++) {
        tree_node_info *current_halo = neighbour_list_start[i_snap];
        while(current_halo != NULL) {
            // Process each new merger
            if(check_merger_type_local(trees, current_halo) > 0)
                n_mergers++;
            current_halo = current_halo->next_neighbour;
        }
    }
    SID_log("Done.", SID_LOG_CLOSE);

    // Initialize the treenode list structures
    treenode_list_info *list_halos;
    init_treenode_list("mergers", n_mergers, &list_halos);
    int *secondary_peak_idx;
    init_treenode_info_data(list_halos, SID_FARG secondary_peak_idx, SID_INT, "Secondary peak index");
    int *secondary_peak_file;
    init_treenode_info_data(list_halos, SID_FARG secondary_peak_file, SID_INT, "Secondary peak snapshot");
    int *primary_peak_id;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_id, SID_INT, "Primary   peak id");
    int *primary_peak_idx;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_idx, SID_INT, "Primary   peak index");
    int *primary_peak_file;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_file, SID_INT, "Primary   peak snapshot");
    int *secondary_id;
    init_treenode_info_data(list_halos, SID_FARG secondary_id, SID_INT, "Secondary      id");
    int *secondary_idx;
    init_treenode_info_data(list_halos, SID_FARG secondary_idx, SID_INT, "Secondary      index");
    int *secondary_file;
    init_treenode_info_data(list_halos, SID_FARG secondary_file, SID_INT, "Secondary      snapshot");
    int *primary_id;
    init_treenode_info_data(list_halos, SID_FARG primary_id, SID_INT, "Primary        id");
    int *primary_idx;
    init_treenode_info_data(list_halos, SID_FARG primary_idx, SID_INT, "Primary        index");
    int *primary_file;
    init_treenode_info_data(list_halos, SID_FARG primary_file, SID_INT, "Primary        snapshot");
    int *remnant_id;
    init_treenode_info_data(list_halos, SID_FARG remnant_id, SID_INT, "Remnant        id");
    int *remnant_idx;
    init_treenode_info_data(list_halos, SID_FARG remnant_idx, SID_INT, "Remnant        index");
    int *remnant_file;
    init_treenode_info_data(list_halos, SID_FARG remnant_file, SID_INT, "Remnant        snapshot");
    double *sig_v_s;
    init_treenode_info_data(list_halos, SID_FARG sig_v_s, SID_DOUBLE, "Secondary velocity dispersion [km/s]");
    double *sig_v_p;
    init_treenode_info_data(list_halos, SID_FARG sig_v_p, SID_DOUBLE, "Primary   velocity dispersion [km/s]");
    double *v_rel;
    init_treenode_info_data(list_halos, SID_FARG v_rel, SID_DOUBLE, "Relative  velocity [km/s]");
    double *xi;
    init_treenode_info_data(list_halos, SID_FARG xi, SID_DOUBLE, "xi");
    double *secondary_peak_Mvir;
    init_treenode_info_data(list_halos, SID_FARG secondary_peak_Mvir, SID_DOUBLE, "Secondary peak M_vir [M_sol/h]");
    double *primary_peak_Mvir;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_Mvir, SID_DOUBLE, "Primary   peak M_vir [M_sol/h]");
    double *remnant_Mvir;
    init_treenode_info_data(list_halos, SID_FARG remnant_Mvir, SID_DOUBLE, "Remnant        M_vir [M_sol/h]");
    int *secondary_peak_n_p_inc;
    init_treenode_info_data(list_halos, SID_FARG secondary_peak_n_p_inc, SID_INT, "Secondary peak inclusive n_p");
    int *primary_peak_n_p_inc;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_n_p_inc, SID_INT, "Primary   peak inclusive n_p");
    int *secondary_n_p_inc;
    init_treenode_info_data(list_halos, SID_FARG secondary_n_p_inc, SID_INT, "Secondary      inclusive n_p");
    int *primary_n_p_inc;
    init_treenode_info_data(list_halos, SID_FARG primary_n_p_inc, SID_INT, "Primary        inclusive n_p");
    int *remnant_n_p_inc;
    init_treenode_info_data(list_halos, SID_FARG remnant_n_p_inc, SID_INT, "Remnant        inclusive n_p");
    int *secondary_peak_n_p;
    init_treenode_info_data(list_halos, SID_FARG secondary_peak_n_p, SID_INT, "Secondary peak n_p");
    int *primary_peak_n_p;
    init_treenode_info_data(list_halos, SID_FARG primary_peak_n_p, SID_INT, "Primary   peak n_p");
    int *secondary_n_p;
    init_treenode_info_data(list_halos, SID_FARG secondary_n_p, SID_INT, "Secondary      n_p");
    int *primary_n_p;
    init_treenode_info_data(list_halos, SID_FARG primary_n_p, SID_INT, "Primary        n_p");
    int *remnant_n_p;
    init_treenode_info_data(list_halos, SID_FARG remnant_n_p, SID_INT, "Remnant        n_p");
    double *z_peak;
    init_treenode_info_data(list_halos, SID_FARG z_peak, SID_DOUBLE, "z_peak");

    // Create the list
    SID_log("Creating lists...", SID_LOG_OPEN | SID_LOG_TIMER);
    int i_list = 0;
    for(int i_snap = 0; i_snap < trees->n_snaps; i_snap++) {
        tree_node_info *current_halo = neighbour_list_start[i_snap];
        while(current_halo != NULL) {
            // Process each new merger
            int type_merger = check_merger_type_local(trees, current_halo);
            if(type_merger > 0) {
                // Set the halos involved in the merger
                double          sig_v_p_i;
                double          sig_v_s_i;
                double          v_rel_i;
                double          xi_i;
                tree_node_info *remnant_halo   = NULL;
                tree_node_info *secondary_halo = current_halo;
                tree_node_info *primary_halo   = NULL;
                // These are actual mergers in the trees.  They must have descendant set
                //    if they made it here.
                if(type_merger == 1) {
                    remnant_halo = current_halo->descendant;
                    primary_halo = current_halo->descendant->progenitor_primary;
                }
                // These are subhalos that are satellites in the last snapshot.  Assume that they
                //    merge with their centrals.  They will stick-out in the output because they
                //    don't have valid remnants.
                else {
                    remnant_halo = NULL;
                    primary_halo = current_halo->parent_top->substructure_first;
                }

                // Set peak-halo points
                tree_node_info *secondary_halo_peak = secondary_halo; // temporary; will be replaced by peak-mass halo by fetch_treenode_merger_info()
                tree_node_info *primary_halo_peak   = primary_halo;   // temporary; will be replaced by peak-mass halo by fetch_treenode_merger_info()
                fetch_treenode_merger_info(trees, &secondary_halo_peak, &primary_halo_peak, &xi_i, &v_rel_i, &sig_v_p_i, &sig_v_s_i);

                // Set merger data
                secondary_peak_idx[i_list]     = fetch_treenode_file_index(trees, secondary_halo_peak);
                secondary_peak_file[i_list]    = fetch_treenode_snapshot(trees, secondary_halo_peak);
                primary_peak_id[i_list]        = fetch_treenode_halo_ID(trees, primary_halo_peak);
                primary_peak_idx[i_list]       = fetch_treenode_file_index(trees, primary_halo_peak);
                primary_peak_file[i_list]      = fetch_treenode_snapshot(trees, primary_halo_peak);
                secondary_id[i_list]           = fetch_treenode_halo_ID(trees, secondary_halo);
                secondary_idx[i_list]          = fetch_treenode_file_index(trees, secondary_halo);
                secondary_file[i_list]         = fetch_treenode_snapshot(trees, secondary_halo);
                primary_id[i_list]             = fetch_treenode_halo_ID(trees, primary_halo);
                primary_idx[i_list]            = fetch_treenode_file_index(trees, primary_halo);
                primary_file[i_list]           = fetch_treenode_snapshot(trees, primary_halo);
                remnant_id[i_list]             = fetch_treenode_halo_ID(trees, remnant_halo);
                remnant_idx[i_list]            = fetch_treenode_file_index(trees, remnant_halo);
                remnant_file[i_list]           = fetch_treenode_snapshot(trees, remnant_halo);
                sig_v_p[i_list]                = sig_v_p_i;
                sig_v_s[i_list]                = sig_v_s_i;
                v_rel[i_list]                  = v_rel_i;
                xi[i_list]                     = xi_i;
                secondary_peak_Mvir[i_list]    = fetch_treenode_M_vir(trees, secondary_halo_peak);
                primary_peak_Mvir[i_list]      = fetch_treenode_M_vir(trees, primary_halo_peak);
                remnant_Mvir[i_list]           = fetch_treenode_M_vir(trees, remnant_halo);
                remnant_n_p[i_list]            = fetch_treenode_n_particles(trees, remnant_halo);
                secondary_n_p[i_list]          = fetch_treenode_n_particles(trees, secondary_halo);
                primary_n_p[i_list]            = fetch_treenode_n_particles(trees, primary_halo);
                secondary_peak_n_p[i_list]     = fetch_treenode_n_particles_peak(trees, secondary_halo_peak);
                primary_peak_n_p[i_list]       = fetch_treenode_n_particles_peak(trees, primary_halo_peak);
                secondary_peak_n_p_inc[i_list] = fetch_treenode_n_particles_inclusive(trees, secondary_halo_peak);
                primary_peak_n_p_inc[i_list]   = fetch_treenode_n_particles_inclusive(trees, primary_halo_peak);
                secondary_n_p_inc[i_list]      = fetch_treenode_n_particles_inclusive(trees, secondary_halo);
                primary_n_p_inc[i_list]        = fetch_treenode_n_particles_inclusive(trees, primary_halo);
                remnant_n_p_inc[i_list]        = fetch_treenode_n_particles_inclusive(trees, remnant_halo);
                z_peak[i_list]                 = fetch_treenode_redshift(trees, secondary_halo_peak);

                // Add halos to lists
                add_to_treenode_list(list_halos, current_halo);

                i_list++;
            }
            current_halo = current_halo->next_neighbour;
        }
    }

    // Finalize lists
    finalize_treenode_list(trees, list_halos);
    SID_log("Done.", SID_LOG_CLOSE);

    // Write the files
    write_treenode_list_markers(trees, filename_out_root, list_halos);
    write_treenode_list_properties(trees, filename_out_root, list_halos);
    write_treenode_list_data(trees, filename_out_root, list_halos);
    // write_treenode_list_hist      (trees,filename_out_root,list_halos,logM_min,dlogM,n_logM);

    // Clean-up
    free_treenode_list(&list_halos);

    SID_log("Done.", SID_LOG_CLOSE);
}
