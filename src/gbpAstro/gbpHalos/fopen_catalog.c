#include <string.h>
#include <gbpLib.h>
#include <gbpHalos.h>

int fopen_nth_catalog_file(fp_catalog_info *fp_in, int n) {
    // Check that the file number makes sense
    // if(fp_in->n_files<(n+1))
    if(fp_in->n_files < n)
        SID_exit_error("Invalid file number (%d) requested for catalog {%s;n_files=%d}.", n,
                       fp_in->filename_properties_base, fp_in->n_files);

    // We can't just jump to the file we want.  We need to keep scaning through them so we know what absolute halo range the n'th file represents
    int i_file;
    int r_val = GBP_FALSE;

    // Start from the beginning if we are going backwards in the file count
    if(n < fp_in->i_file) {
        i_file              = 0;
        fp_in->i_halo_start = 0;
    }
    // We have to scan all files in order, so if we're jumping more than one,
    //    start with the next one first and we'll get there
    else if(n > fp_in->i_file)
        i_file = fp_in->i_file + 1;
    else
        i_file = fp_in->i_file;

    // Loop until we get to the file we want (or there's an error)
    while(i_file <= n && !r_val) {
        char filename_properties[SID_MAX_FILENAME_LENGTH];
        char filename_profiles[SID_MAX_FILENAME_LENGTH];

        // Close the file pointers if they are already open
        if(fp_in->fp_properties != NULL) {
            fclose(fp_in->fp_properties);
            fp_in->fp_properties = NULL;
        }
        if(fp_in->fp_profiles != NULL) {
            fclose(fp_in->fp_profiles);
            fp_in->fp_profiles = NULL;
        }

        // Create the filename
        if(fp_in->flag_multifile) {
            sprintf(filename_properties, "%s/%s.%d", fp_in->filename_properties_root, fp_in->filename_properties_base, i_file);
            sprintf(filename_profiles, "%s/%s.%d", fp_in->filename_profiles_root, fp_in->filename_profiles_base, i_file);
        } else {
            if(i_file == 0) {
                sprintf(filename_properties, "%s", fp_in->filename_properties_root);
                sprintf(filename_profiles, "%s", fp_in->filename_profiles_root);
            } else
                SID_exit_error(
                        "Catalog file identified as non-multi-file {%s} has been accessed as multi-file {requested file=%d}.",
                        SID_ERROR_LOGIC, fp_in->filename_properties_base, i_file);
        }

        // Try to open properties file
        if(fp_in->flag_read_properties) {
            if(((fp_in->fp_properties) = fopen(filename_properties, "r")) == NULL)
                r_val = GBP_TRUE;
            else {
                // Read header information
                SID_fread_verify(&(fp_in->i_file), sizeof(int), 1, fp_in->fp_properties);
                SID_fread_verify(&(fp_in->n_files), sizeof(int), 1, fp_in->fp_properties);
                SID_fread_verify(&(fp_in->n_halos_file), sizeof(int), 1, fp_in->fp_properties);
                SID_fread_verify(&(fp_in->n_halos_total), sizeof(int), 1, fp_in->fp_properties);
                // Check that the file number in the file is correct
                if(i_file != fp_in->i_file)
                    SID_exit_error("Invalid file number (ie. %d!=%d) in catalog {%s}.", SID_ERROR_LOGIC, i_file,
                                   fp_in->i_file, fp_in->filename_properties_root);
            }
        }

        // Try to open profiles file
        if(fp_in->flag_read_profiles) {
            if(((fp_in->fp_profiles) = fopen(filename_profiles, "r")) == NULL)
                r_val = GBP_TRUE;
            else {
                // Read header information
                SID_fread_verify(&(fp_in->i_file), sizeof(int), 1, fp_in->fp_profiles);
                SID_fread_verify(&(fp_in->n_files), sizeof(int), 1, fp_in->fp_profiles);
                SID_fread_verify(&(fp_in->n_halos_file), sizeof(int), 1, fp_in->fp_profiles);
                SID_fread_verify(&(fp_in->n_halos_total), sizeof(int), 1, fp_in->fp_profiles);
                // Check that the file number in the file is correct
                if(i_file != fp_in->i_file)
                    SID_exit_error("Invalid file number (ie. %d!=%d) in catalog {%s}.", SID_ERROR_LOGIC, i_file,
                                   fp_in->i_file, fp_in->filename_profiles_root);
            }
        }

        // Set the absolute start and stop ranges of the halo numbers
        if((fp_in->i_file) == 0)
            fp_in->i_halo_start = 0;
        else
            fp_in->i_halo_start = fp_in->i_halo_stop + 1;
        fp_in->i_halo_stop = fp_in->i_halo_start + fp_in->n_halos_file - 1;
        fp_in->i_halo      = fp_in->i_halo_start; // This should always be the halo index pointed to by the present state of the file pointers

        i_file++;
    }

    if(r_val == GBP_TRUE)
        SID_exit_error("Problem encountered opening file {%s/%s;file=%d}", SID_ERROR_LOGIC,
                       fp_in->filename_properties_base, fp_in->filename_profiles_base, n);

    return (r_val);
}

int fopen_catalog(char *filename_catalog_root, int snapshot_number, int mode, fp_catalog_info *fp_out) {
    char filename_root[SID_MAX_FILENAME_LENGTH];
    int  r_val          = GBP_FALSE;
    int  flag_filefound = GBP_FALSE;
    char group_text_prefix[8];

    // Decide whether we're opening a group or subgroup catalog
    int flag_read_groups    = GBP_FALSE;
    int flag_read_subgroups = GBP_FALSE;
    if(SID_CHECK_BITFIELD_SWITCH(mode, READ_CATALOG_GROUPS)) {
        flag_read_groups    = GBP_TRUE;
        flag_read_subgroups = GBP_FALSE;
        sprintf(group_text_prefix, "");
    }
    if(SID_CHECK_BITFIELD_SWITCH(mode, READ_CATALOG_SUBGROUPS)) {
        if(flag_read_groups)
            SID_exit_error("You can't open both groups and subgroups at the same time in fopen_catalog().",
                           SID_ERROR_LOGIC);
        flag_read_groups    = GBP_FALSE;
        flag_read_subgroups = GBP_TRUE;
        sprintf(group_text_prefix, "sub");
    }
    if(!flag_read_groups && !flag_read_subgroups)
        SID_exit_error(
                "You must specify either READ_CATALOG_GROUPS or READ_CATALOG_SUBGROUPS in mode for fopen_catalog().",
                SID_ERROR_LOGIC);

    // Decide if we are reading properties
    if(SID_CHECK_BITFIELD_SWITCH(mode, READ_CATALOG_PROPERTIES))
        fp_out->flag_read_properties = GBP_TRUE;
    else
        fp_out->flag_read_properties = GBP_FALSE;

    // Decide if we are reading profiles
    if(SID_CHECK_BITFIELD_SWITCH(mode, READ_CATALOG_PROFILES))
        fp_out->flag_read_profiles = GBP_TRUE;
    else
        fp_out->flag_read_profiles = GBP_FALSE;

    // Make sure we're reading something
    if(fp_out->flag_read_properties == GBP_FALSE && fp_out->flag_read_profiles == GBP_FALSE)
        SID_exit_error("Neither properties nor profiles are selected for reading in fopen_catalog().", SID_ERROR_LOGIC);

    // Set snapshot number
    fp_out->snap_num = snapshot_number;

    // Sort out what file format we're working with
    fp_out->fp_properties = NULL;
    fp_out->fp_profiles   = NULL;
    if(SID.I_am_Master) {
        int  i_file;
        char filename_properties[SID_MAX_FILENAME_LENGTH];
        char filename_profiles[SID_MAX_FILENAME_LENGTH];

        // Set some filename information
        sprintf(fp_out->filename_properties_root, "%s_%03d.catalog_%sgroups_properties", filename_catalog_root, snapshot_number, group_text_prefix);
        sprintf(fp_out->filename_profiles_root, "%s_%03d.catalog_%sgroups_profiles", filename_catalog_root, snapshot_number, group_text_prefix);
        strcpy(fp_out->filename_properties_base, fp_out->filename_properties_root);
        strcpy(fp_out->filename_profiles_base, fp_out->filename_profiles_root);
        strip_path(fp_out->filename_properties_base);
        strip_path(fp_out->filename_profiles_base);

        // Try reading a multifile first ...
        sprintf(filename_properties, "%s/%s.%d", fp_out->filename_properties_root, fp_out->filename_properties_base, 0);
        fp_out->fp_properties = fopen(filename_properties, "r");
        if(fp_out->fp_properties == NULL) {
            sprintf(filename_properties, "%s", fp_out->filename_properties_root);
            // ... if we didn't find a multi-file, try reading a single file ...
            fp_out->fp_properties = fopen(filename_properties, "r");
            if(fp_out->fp_properties == NULL) {
                r_val = GBP_TRUE;
                SID_exit_error("Could not open catalog {%s} snapshot #%03d {%s}.", SID_ERROR_IO_OPEN,
                               filename_catalog_root, snapshot_number, filename_properties);
            }
            // ... we found a single file.  Set flags.
            else
                fp_out->flag_multifile = GBP_FALSE;
        }
        // ... we found a multi-file.  Set flags.
        else
            fp_out->flag_multifile = GBP_TRUE;

        // Load/set header information
        if(fp_out->fp_properties != NULL) {
            SID_fread_verify(&(fp_out->i_file), sizeof(int), 1, fp_out->fp_properties);
            SID_fread_verify(&(fp_out->n_files), sizeof(int), 1, fp_out->fp_properties);
            SID_fread_verify(&(fp_out->n_halos_file), sizeof(int), 1, fp_out->fp_properties);
            SID_fread_verify(&(fp_out->n_halos_total), sizeof(int), 1, fp_out->fp_properties);
            fclose(fp_out->fp_properties);
            fp_out->fp_properties = NULL;
        }
    }
    SID_Bcast(fp_out, sizeof(fp_catalog_info), SID_CHAR, SID_MASTER_RANK, SID_COMM_WORLD);

    // Initialize things by opening the first file.  Even if we want a halo
    //   that's deep in the list, we have to scan all the headers (starting
    //   with the first) to find where it is.
    int r_val2;
    fp_out->fp_properties = NULL;
    fp_out->fp_profiles   = NULL;
    fp_out->i_file        = 0;
    fp_out->i_halo        = 0;
    if((r_val2 = fopen_nth_catalog_file(fp_out, 0)))
        SID_exit_error("Error opening catalog file.", SID_ERROR_IO_OPEN);
    if(r_val2 > r_val)
        r_val = r_val2;

    return (r_val);
}
