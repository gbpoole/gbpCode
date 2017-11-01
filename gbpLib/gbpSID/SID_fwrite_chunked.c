#include <gbpSID.h>
#include <string.h>

size_t SID_fwrite_chunked(void *buffer, size_t n_x_write_local, size_t i_x_offset_local, SID_fp *fp) {
    int                    i_chunk;
    int                    i_rank;
    size_t                 i_x_write_chunk;
    size_t                 i_x_offset_global;
    size_t                 i_x_chunk;
    size_t                 n_skip;
    size_t                 n_x_chunk;
    size_t                 n_x_chunk_max;
    size_t                 header_size;
    size_t                 r_val = 0;
    size_t                 n_skip_header;
    size_t                 n_write_header1;
    size_t                 n_write_header2;
    size_t                 n_write_subheader;
    char                   filename_chunk[256];
    chunked_subheader_info chunked_subheader;
    i_x_offset_global = fp->last_item;
    for(i_chunk = 0, i_x_write_chunk = 0, i_x_chunk = i_x_offset_local + i_x_offset_global; i_chunk < fp->chunked_header.n_chunk; i_chunk++) {
        if(fp->i_x_start_chunk[i_chunk] <= i_x_chunk && fp->i_x_last_chunk[i_chunk] >= i_x_chunk) {
            n_skip    = i_x_chunk - fp->i_x_start_chunk[i_chunk];
            n_x_chunk = GBP_MIN(n_x_write_local - i_x_write_chunk, fp->i_x_step_chunk[i_chunk] - n_skip);
        } else {
            n_x_chunk = 0;
            n_skip    = 0;
        }
        SID_Allreduce(&n_x_chunk, &n_x_chunk_max, 1, SID_SIZE_T, SID_MAX, SID.COMM_WORLD);
        if(n_x_chunk_max > 0) {
            sprintf(filename_chunk, "%s.%d", fp->filename_root, i_chunk);
#if !USE_MPI_IO
            for(i_rank = 0; i_rank < SID.n_proc; i_rank++) {
                if(SID.My_rank == i_rank && n_x_chunk > 0) {
#endif
                    SID_fopen(filename_chunk, "a", fp);
                    if(n_skip == 0) {
                        if(i_chunk == 0) {
                            SID_fwrite(&(fp->chunked_header), sizeof(chunked_header_info), 1, fp);
                            if(fp->header != NULL && fp->chunked_header.header_size > 0)
                                SID_fwrite(fp->header, fp->chunked_header.header_size, 1, fp);
                        }
                        chunked_subheader.i_chunk = i_chunk;
                        chunked_subheader.n_items = fp->i_x_step_chunk[i_chunk];
                        SID_fwrite(&chunked_subheader, sizeof(chunked_subheader_info), 1, fp);
                    }
#if USE_MPI_IO
                    else
                        SID_fseek(1, fp->header_offset[i_chunk] + n_skip * fp->chunked_header.item_size, fp);
#endif
                    if(n_x_chunk > 0) {
                        SID_fwrite((char *)buffer + i_x_write_chunk * fp->chunked_header.item_size, fp->chunked_header.item_size, n_x_chunk, fp);
                        i_x_chunk += n_x_chunk;
                        i_x_write_chunk += n_x_chunk;
                    }
                    SID_fclose(fp);
#if !USE_MPI_IO
                }
                SID_Barrier(SID.COMM_WORLD);
            }
#endif
        }
    }
    SID_Allreduce(&i_x_chunk, &(fp->last_item), 1, SID_SIZE_T, SID_MAX, SID.COMM_WORLD);
    // fp->last_item--;
    return (r_val);
}
