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
#include "msh.h"
#include "io.h"
#include "xmf.h"

//voxel ap
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    //make dir
    mkdir(ROOT_WRITE, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    /*
     =============================
     init
     =============================
     */
    
    //ocl
    struct ocl_obj ocl;
    ocl_ini(&ocl);
    
    //mesh
    struct msh_obj msh;
    msh.nt = 100;
    msh.dt = 1e2f;
    msh.dx = 1e-0f;
    msh.ele.dim = (cl_int3){10,1,100};
    msh_ini(&msh);
    
    //memory
    cl_mem gg = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_int),    NULL, &ocl.err);
    cl_mem uu = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_float2), NULL, &ocl.err);
    cl_mem bb = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_float2), NULL, &ocl.err);

    //kernels
    cl_kernel ele_ini = clCreateKernel(ocl.program, "ele_ini", &ocl.err);
    cl_kernel ele_exp = clCreateKernel(ocl.program, "ele_exp", &ocl.err);
    cl_kernel ele_jac = clCreateKernel(ocl.program, "ele_jac", &ocl.err);
    cl_kernel ele_rhs = clCreateKernel(ocl.program, "ele_rhs", &ocl.err);
    

    //args
    ocl.err = clSetKernelArg(ele_ini, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_ini, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_ini, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(ele_ini, 3, sizeof(cl_mem),           (void*)&bb);
    
    ocl.err = clSetKernelArg(ele_exp, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_exp, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_exp, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(ele_exp, 3, sizeof(cl_mem),           (void*)&bb);
    
    ocl.err = clSetKernelArg(ele_jac, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_jac, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_jac, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(ele_jac, 3, sizeof(cl_mem),           (void*)&bb);
    
    ocl.err = clSetKernelArg(ele_rhs, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_rhs, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_rhs, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(ele_rhs, 3, sizeof(cl_mem),           (void*)&bb);
    
    
    //init
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_ini, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);
    
    //write
    file_write(&ocl, "gg", &gg, msh.ele.tot, sizeof(cl_int), 0);
    
    //frames
    for(int i=0; i<msh.nt; i++)
    {
//        printf("i %02d\n", i);
        
        //write
        write_xmf(&msh, i);
        file_write(&ocl, "uu", &uu, msh.ele.tot, sizeof(cl_float2), i);
        
        //ee
//      ocl.err = clSetKernelArg(ele_exp, 4, sizeof(int), (void*)&t);
//      ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_exp, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);
        
        //ie rhs
//      ocl.err = clEnqueueCopyBuffer(ocl.command_queue, uu, bb, 0, 0, msh.ele.tot*sizeof(cl_float2), 0, NULL, &ocl.event);
        ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_rhs, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);     //pump
        
        //ie jacobi
        for(int k=0; k<10; k++)
        {
            ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_jac, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);
        }
    }


    /*
     =============================
     final
     =============================
     */
    
    //queue
    ocl.err = clFlush(ocl.command_queue);
    ocl.err = clFinish(ocl.command_queue);
    
    //memory
    ocl.err = clReleaseMemObject(gg);
    ocl.err = clReleaseMemObject(uu);
    ocl.err = clReleaseMemObject(bb);

    //kernels
    ocl.err = clReleaseKernel(ele_ini);
    ocl.err = clReleaseKernel(ele_exp);
    ocl.err = clReleaseKernel(ele_jac);
    ocl.err = clReleaseKernel(ele_rhs);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
