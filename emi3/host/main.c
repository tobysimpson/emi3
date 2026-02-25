//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>

#include "ocl.h"
#include "vxl.h"
#include "io.h"
#include "xdmf.h"


//voxel diffusion/pump
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    //make dir
    mkdir("/Users/toby/Downloads/vxl", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    /*
     =============================
     init
     =============================
     */
    
    //ocl
    struct ocl_obj ocl;
    ocl_ini(&ocl);
    
    struct vxl_obj vxl;
    vxl.dt = 0.1f;
    vxl.dx = 1.0f;
    vxl.ele.dim = (cl_int3){8,8,8};
//    vxl.ne = (cl_int3){67,14,14};
    vxl_ini(&vxl);
    
    //memory
    cl_mem gg = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ele.tot*sizeof(cl_int),    NULL, &ocl.err);
    cl_mem uu = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ele.tot*sizeof(cl_float2), NULL, &ocl.err);
    cl_mem bb = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ele.tot*sizeof(cl_float2), NULL, &ocl.err);
    
    //read
//    file_read(&ocl, "vxl_tag.dat", &gg, vxl.ne_tot, sizeof(cl_float));
    
    //kernels
    cl_kernel vxl_ini = clCreateKernel(ocl.program, "vxl_ini", &ocl.err);
    cl_kernel vxl_exp = clCreateKernel(ocl.program, "vxl_exp", &ocl.err);
    cl_kernel vxl_jac = clCreateKernel(ocl.program, "vxl_jac", &ocl.err);
    
    //args
    ocl.err = clSetKernelArg(vxl_ini, 0, sizeof(struct vxl_obj),   (void*)&vxl);
    ocl.err = clSetKernelArg(vxl_ini, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(vxl_ini, 2, sizeof(cl_mem),           (void*)&uu);
    
    ocl.err = clSetKernelArg(vxl_exp, 0, sizeof(struct vxl_obj),   (void*)&vxl);
    ocl.err = clSetKernelArg(vxl_exp, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(vxl_exp, 2, sizeof(cl_mem),           (void*)&uu);
    
    ocl.err = clSetKernelArg(vxl_jac, 0, sizeof(struct vxl_obj),   (void*)&vxl);
    ocl.err = clSetKernelArg(vxl_jac, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(vxl_jac, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(vxl_jac, 3, sizeof(cl_mem),           (void*)&bb);
    
    //init
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_ini, 3, NULL, (size_t*)&vxl.ele.sz, NULL, 0, NULL, &ocl.event);
    
    //write
    file_write(&ocl, "gg", &gg, vxl.ele.tot, sizeof(cl_int), 0);
    
    //frames
    for(int frm_idx=0; frm_idx<100; frm_idx++)
    {
        printf("frm %02d\n", frm_idx);
        
        //write
        write_xmf(&vxl, frm_idx);
        file_write(&ocl, "uu", &uu, vxl.ele.tot, sizeof(cl_float2), frm_idx);
        
        //time per fame
        for(int t=0; t<1; t++)
        {
            //ee
//            ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_exp, 3, NULL, (size_t*)&vxl.ele.sz, NULL, 0, NULL, &ocl.event);
            
            //ie rhs
            ocl.err = clEnqueueCopyBuffer(ocl.command_queue, uu, bb, 0, 0, vxl.ele.tot*sizeof(cl_float2), 0, NULL, &ocl.event);
            
            //ie jacobi
            for(int t=0; t<10; t++)
            {
                ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_jac, 3, NULL, (size_t*)&vxl.ele.sz, NULL, 0, NULL, &ocl.event);
            }
        }
    }


    /*
     =============================
     final
     =============================
     */
    
    //memory
    ocl.err = clReleaseMemObject(gg);
    ocl.err = clReleaseMemObject(uu);
    ocl.err = clReleaseMemObject(bb);

    //kernels
    ocl.err = clReleaseKernel(vxl_ini);
    ocl.err = clReleaseKernel(vxl_exp);
    ocl.err = clReleaseKernel(vxl_jac);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
