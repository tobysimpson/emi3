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
    msh.dt = 5e-2f;
    msh.dx = 1e-0f;
    msh.ele.dim = (cl_int3){2,1,16};
    msh_ini(&msh);
    
    //memory
    cl_mem gg = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_int),    NULL, &ocl.err);
    cl_mem uu = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_float2), NULL, &ocl.err);
    cl_mem bb = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, msh.ele.tot*sizeof(cl_float2), NULL, &ocl.err);

    
    //read
//    file_read(&ocl, "vxl_tag.dat", &gg, vxl.ne_tot, sizeof(cl_float));
    
    //kernels
    cl_kernel ele_ini = clCreateKernel(ocl.program, "ele_ini", &ocl.err);
    cl_kernel ele_exp = clCreateKernel(ocl.program, "ele_exp", &ocl.err);
    cl_kernel vxl_rhs = clCreateKernel(ocl.program, "vxl_rhs", &ocl.err);
    cl_kernel vxl_jac = clCreateKernel(ocl.program, "vxl_jac", &ocl.err);
    
    //args
    ocl.err = clSetKernelArg(ele_ini, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_ini, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_ini, 2, sizeof(cl_mem),           (void*)&uu);
    
    ocl.err = clSetKernelArg(ele_exp, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(ele_exp, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(ele_exp, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(ele_exp, 3, sizeof(cl_mem),           (void*)&bb);
    
    ocl.err = clSetKernelArg(vxl_rhs, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(vxl_rhs, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(vxl_rhs, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(vxl_rhs, 3, sizeof(cl_mem),           (void*)&bb);
    
    ocl.err = clSetKernelArg(vxl_jac, 0, sizeof(struct msh_obj),   (void*)&msh);
    ocl.err = clSetKernelArg(vxl_jac, 1, sizeof(cl_mem),           (void*)&gg);
    ocl.err = clSetKernelArg(vxl_jac, 2, sizeof(cl_mem),           (void*)&uu);
    ocl.err = clSetKernelArg(vxl_jac, 3, sizeof(cl_mem),           (void*)&bb);
    
    
    //init
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_ini, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);
    
    //write
    file_write(&ocl, "gg", &gg, msh.ele.tot, sizeof(cl_int), 0);
    
    //frames
    for(int frm_idx=0; frm_idx<400; frm_idx++)
    {
//        printf("frm %02d\n", frm_idx);
        
        //write
        write_xmf(&msh, frm_idx);
        file_write(&ocl, "uu", &uu, msh.ele.tot, sizeof(cl_float2), frm_idx);
        
        //time per fame
        for(int t=0; t<1; t++)
        {
            //ee
//            ocl.err = clSetKernelArg(vxl_exp, 4, sizeof(int), (void*)&frm_idx);
            ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_exp, 3, NULL, (size_t*)&msh.ele.sz, NULL, 0, NULL, &ocl.event);
            
            //ie rhs
//            ocl.err = clEnqueueCopyBuffer(ocl.command_queue, uu, bb, 0, 0, vxl.ele.tot*sizeof(cl_float2), 0, NULL, &ocl.event);
//            ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_rhs, 3, NULL, (size_t*)&vxl.ele.sz, NULL, 0, NULL, &ocl.event); //use with jacobi
            
//            //ie jacobi
//            for(int t=0; t<10; t++)
//            {
//                ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_jac, 3, NULL, (size_t*)&vxl.ele.sz, NULL, 0, NULL, &ocl.event);
//            }
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
    ocl.err = clReleaseKernel(vxl_rhs);
    ocl.err = clReleaseKernel(vxl_jac);
    

    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
