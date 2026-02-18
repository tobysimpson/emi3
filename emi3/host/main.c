//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ocl.h"
#include "vxl.h"
#include "io.h"
#include "xdmf.h"


//voxel diffusion
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    /*
     =============================
     init
     =============================
     */
    
    //ocl
    struct ocl_obj ocl;
    ocl_ini(&ocl);
    
    struct vxl_obj vxl;
    vxl.dt = 1.0f;
    vxl.dx = 10e0f;
    vxl.ne = (cl_int3){67,14,14};
    vxl.x0 = (cl_float3){0e0f,0e0f,0e0f};
    vxl.x1 = (cl_float3){664.59f,139.78f,139.92f};
    vxl_ini(&vxl);
    

    //memory
    cl_mem vxl_tag = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ne_tot*sizeof(cl_float),  NULL, &ocl.err);
    cl_mem vxl_dat = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ne_tot*sizeof(cl_float2), NULL, &ocl.err);
    
    //read
    file_read(&ocl, "vxl_tag.dat", &vxl_tag, vxl.ne_tot, sizeof(cl_float));

    //kernels
    cl_kernel vxl_ini = clCreateKernel(ocl.program, "vxl_ini", &ocl.err);
    cl_kernel vxl_ion = clCreateKernel(ocl.program, "vxl_ion", &ocl.err);

    //args
    ocl.err = clSetKernelArg(vxl_ini,  0, sizeof(struct vxl_obj),   (void*)&vxl);
    ocl.err = clSetKernelArg(vxl_ini,  1, sizeof(cl_mem),           (void*)&vxl_dat);
    ocl.err = clSetKernelArg(vxl_ini,  2, sizeof(cl_mem),           (void*)&vxl_tag);
    
    ocl.err = clSetKernelArg(vxl_ion,  0, sizeof(struct vxl_obj),   (void*)&vxl);
    ocl.err = clSetKernelArg(vxl_ion,  1, sizeof(cl_mem),           (void*)&vxl_dat);
    ocl.err = clSetKernelArg(vxl_ion,  2, sizeof(cl_mem),           (void*)&vxl_tag);
    
    
    //run
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_ini, 3, NULL, (size_t*)&vxl.ne_sz, NULL, 0, NULL, &ocl.event);
    
    //time
    for(int t=0; t<100; t++)
    {
        //calc
        ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_ion, 3, NULL, (size_t*)&vxl.ne_sz, NULL, 0, NULL, &ocl.event);
        
        //write
        write_xmf(&vxl, t);
        
        char file_name[250];
        sprintf(file_name, "vxl_dat.%03d.dat", t);
        file_write(&ocl, file_name, &vxl_dat, vxl.ne_tot, sizeof(cl_float2));
    }


    /*
     =============================
     final
     =============================
     */
    
    //memory
    ocl.err = clReleaseMemObject(vxl_tag);
    ocl.err = clReleaseMemObject(vxl_dat);

    //kernels
    ocl.err = clReleaseKernel(vxl_ini);
    ocl.err = clReleaseKernel(vxl_ion);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
