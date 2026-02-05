//
//  ocl.h
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//

#ifndef ocl_h
#define ocl_h


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif



//object
struct ocl_obj
{
    //environment
    cl_int              err;
    cl_platform_id      platform_id;
    cl_device_id        device_id;
    cl_uint             num_devices;
    cl_uint             num_platforms;
    cl_context          context;
    cl_command_queue    command_queue;
    cl_program          program;
    cl_event            event;
    
    //debug
    char                device_char[50];
    cl_uint             device_uint;
};


//init
void ocl_ini(struct ocl_obj *ocl)
{
    /*
     =============================
     environment
     =============================
     */
    
    ocl->err            = clGetPlatformIDs(1, &ocl->platform_id, &ocl->num_platforms);                                  //platform
    ocl->err            = clGetDeviceIDs(ocl->platform_id, CL_DEVICE_TYPE_GPU, 1, &ocl->device_id, &ocl->num_devices);  //devices
    ocl->context        = clCreateContext(NULL, ocl->num_devices, &ocl->device_id, NULL, NULL, &ocl->err);              //context
    ocl->command_queue  = clCreateCommandQueue(ocl->context, ocl->device_id, 0, &ocl->err);     //command queue         //CL_QUEUE_PROFILING_ENABLE
    
    ocl->err            = clGetDeviceInfo(ocl->device_id, CL_DEVICE_NAME,         50, &ocl->device_char, NULL);         //CL_DEVICE_NAME,
//    ocl->err            = clGetDeviceInfo(ocl->device_id, CL_DEVICE_ADDRESS_BITS,  4, &ocl->device_uint, NULL);         //CL_DEVICE_ADDRESS_BITS

    printf("%s\n", ocl->device_char);
//    printf("%u\n", ocl->device_uint);

    /*
     =============================
     program
     =============================
     */
    
    //src
    FILE* src_file = fopen("prg.cl", "r");
    if(!src_file)
    {
        fprintf(stderr, "prg.cl not found\n");
        exit(1);
    }

    //length
    fseek(src_file, 0, SEEK_END);
    size_t src_len =  ftell(src_file);
    fseek(src_file, 0, SEEK_SET);

    //source
    char *src_mem = (char*)malloc(src_len);
    fread(src_mem, sizeof(char), src_len, src_file);
    fclose(src_file);

    //create
    ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&src_mem, (const size_t*)&src_len, &ocl->err);
    printf("prg %d\n",ocl->err);
    
    //build
    ocl->err = clBuildProgram(ocl->program, 1, &ocl->device_id, NULL, NULL, NULL);
    printf("bld %d\n",ocl->err);
    
    //clean
    ocl->err = clUnloadPlatformCompiler(ocl->platform_id);
    free(src_mem);
    
    /*
     =============================
     log
     =============================
     */

    //log
    size_t log_size = 0;
    
    //log size
    clGetProgramBuildInfo(ocl->program, ocl->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

    //allocate
    char *log = (char*)malloc(log_size);

    //log text
    clGetProgramBuildInfo(ocl->program, ocl->device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

    //print
    printf("%s\n", log);

    //clear
    free(log);
}


//final
void ocl_fin(struct ocl_obj *ocl)
{
    //queue
    ocl->err = clFlush(ocl->command_queue);
    ocl->err = clFinish(ocl->command_queue);
    
    //context
    ocl->err = clReleaseProgram(ocl->program);
    ocl->err = clReleaseCommandQueue(ocl->command_queue);
    ocl->err = clReleaseContext(ocl->context);
    
    return;
}


#endif /* ocl_h */
