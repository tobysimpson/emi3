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
#include "vec.h"
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
    vxl.dx = 10e0f;
    vxl.x0 = (cl_float3){0e0f,0e0f,0e0f};
    vxl.x1 = (cl_float3){664.59f,139.78f,139.92f};
    vxl_ini(&vxl);
    

    //memory
    cl_mem vxl_tag = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ne_tot*sizeof(cl_float),    NULL, &ocl.err);
    
    //read
    read_f2b(&ocl, "vxl_tag.dat", &vxl_tag, vxl.ne_tot, sizeof(cl_float));

    //kernels
    cl_kernel vxl_ini = clCreateKernel(ocl.program, "vxl_ini", &ocl.err);

    //args
    ocl.err = clSetKernelArg(vxl_ini,  0, sizeof(struct vxl_obj),(void*)&vxl);
    ocl.err = clSetKernelArg(vxl_ini,  1, sizeof(cl_mem),(void*)&vxl_tag);
    
   
    //run
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_ini, 3, NULL, (size_t*)&vxl.ne_sz, NULL, 0, NULL, &ocl.event);


    /*
     =============================
     write
     =============================
     */
    
    //write
    write_xmf(&vxl, 0);

    /*
     =============================
     final
     =============================
     */
    
    //memory
    ocl.err = clReleaseMemObject(vxl_tag);

    //kernels
    ocl.err = clReleaseKernel(vxl_ini);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
