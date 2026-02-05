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



//write raw
void read_flt4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    FILE*       file_ptr;
    char        file_path[250];
    cl_float4*  host_ptr;
    
    //buffer
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*sizeof(cl_float4), 0, NULL, NULL, &ocl->err);
    
    for (int i=0; i<n; i++)
    {
        fread(&host_ptr[i], sizeof(cl_float), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}


//write raw
void read_int4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    FILE*       file_ptr;
    char        file_path[250];
    cl_int4*    host_ptr;
    
    //buffer
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*sizeof(cl_int4), 0, NULL, NULL, &ocl->err);
    
    for (int i=0; i<n; i++)
    {
        fread(&host_ptr[i], sizeof(cl_int), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}



////write raw
//void wrt_raw(struct ocl_obj *ocl, struct msh_obj *msh, cl_mem *buf, char *dsc, int idx)
//{
//    FILE* file1;
//    char file1_name[250];
//    float* ptr1;
//    
//    //buffer
//    sprintf(file1_name, "%s/raw/%s.%02d%02d.%02d.raw", ROOT_WRITE, dsc, msh->le.x, msh->le.y, idx);
//    file1 = fopen(file1_name,"wb");
//    ptr1 = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, msh->nv_tot*sizeof(float), 0, NULL, NULL, &ocl->err);
//    fwrite(ptr1, sizeof(float), msh->nv_tot, file1);
//    clEnqueueUnmapMemObject(ocl->command_queue, *buf, ptr1, 0, NULL, NULL);
//    
//    //clean up
//    fclose(file1);
//    
//    return;
//}



#endif /* io_h */
