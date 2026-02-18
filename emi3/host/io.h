//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//

#ifndef io_h
#define io_h


#define ROOT_READ   "/Users/toby/Downloads"
#define ROOT_WRITE  "/Users/toby/Downloads"


/*
 =============================
 util
 =============================
 */

//file length in bytes
size_t file_size(char* file_name)
{
    FILE*   file_ptr;
    char    file_path[250];
    
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    fseek(file_ptr, 0, SEEK_END);
    size_t n = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    fclose(file_ptr);
    
    return n;
}

/*
 =============================
 read
 =============================
 */

//file to cl_mem
void file_read(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, size_t w)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    
    FILE* file_ptr = fopen(file_path,"rb");
    void* host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*w, 0, NULL, NULL, &ocl->err);
    
    fread(host_ptr, w, n, file_ptr);
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}

//file to cl_mem
void file_write(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, size_t w)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    
    FILE* file_ptr = fopen(file_path,"wb");
    void* host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*w, 0, NULL, NULL, &ocl->err);
    
    fwrite(host_ptr, w, n, file_ptr);
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}


#endif /* io_h */
