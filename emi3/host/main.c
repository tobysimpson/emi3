//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>

#include "ocl.h"
#include "io.h"

//working with tet mesh
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
    
    
//    write_xmf(0);
    
    //dims
    size_t nv = file_size("vtx_xyz.raw")/(3*sizeof(float));
    size_t ne = file_size("ele_vtx.raw")/(4*sizeof(int));
    size_t nf = file_size("fac_vtx.raw")/(3*sizeof(int));
    
    printf("nv %8ld\n", nv);
    printf("ne %8ld\n", ne);
    printf("nf %8ld\n", nf);
    
    //xdmf
    write_xmf(nv, ne, nf, 0);
    
    //memory
    cl_mem vtx_xyz = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, nv*sizeof(cl_float4), NULL, &ocl.err);
    cl_mem ele_vtx = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, ne*sizeof(cl_int4), NULL, &ocl.err);
    cl_mem fac_vtx = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, nf*sizeof(cl_int4), NULL, &ocl.err);
    
    cl_mem vtx_dat = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, ne*sizeof(cl_int4), NULL, &ocl.err);
    cl_mem ele_dat = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, ne*sizeof(cl_int4), NULL, &ocl.err);
    
    //read
    read_flt4(&ocl, "vtx_xyz.raw", &vtx_xyz, nv, 3);
    read_int4(&ocl, "ele_vtx.raw", &ele_vtx, ne, 4);
    read_int4(&ocl, "fac_vtx.raw", &fac_vtx, nf, 3);
    
    //kernels
    cl_kernel vtx_ini = clCreateKernel(ocl.program, "vtx_ini", &ocl.err);
    cl_kernel ele_ini = clCreateKernel(ocl.program, "ele_ini", &ocl.err);
    cl_kernel fac_ini = clCreateKernel(ocl.program, "fac_ini", &ocl.err);

    //args
    ocl.err = clSetKernelArg(vtx_ini,  0, sizeof(cl_mem),(void*)&vtx_xyz);
    ocl.err = clSetKernelArg(vtx_ini,  1, sizeof(cl_mem),(void*)&vtx_dat);
    
    ocl.err = clSetKernelArg(ele_ini,  0, sizeof(cl_mem),(void*)&ele_vtx);
    ocl.err = clSetKernelArg(ele_ini,  1, sizeof(cl_mem),(void*)&ele_dat);
    
    ocl.err = clSetKernelArg(fac_ini,  0, sizeof(cl_mem),(void*)&fac_vtx);

//    nv = 4;
//    ne = 4;
//    nf = 4;
    
    //run
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vtx_ini, 1, NULL, &nv, NULL, 0, NULL, &ocl.event);
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_ini, 1, NULL, &ne, NULL, 0, NULL, &ocl.event);
//    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, fac_ini, 1, NULL, &nf, NULL, 0, NULL, &ocl.event);
    
    
    //write
    write_int4(&ocl, "vtx_dat.raw", &vtx_dat, nv, 1);
    write_int4(&ocl, "ele_dat.raw", &ele_dat, nv, 1);

//    
//    //map
//    cl_float4* buf = clEnqueueMapBuffer(ocl.command_queue, cc, CL_TRUE, CL_MAP_READ, 0, n*sizeof(cl_float4), 0, NULL, NULL, &ocl.err);
//    
//    //     for(int i=0; i<10; i++)
//    //     {
//    //         printf("%+e\n", buf[i].x);
//    //     }
//    
//    //unmap
//    clEnqueueUnmapMemObject(ocl.command_queue, cc, buf, 0, NULL, NULL);
    
    /*
     =============================
     final
     =============================
     */
    
    //memory
    ocl.err = clReleaseMemObject(vtx_xyz);
    ocl.err = clReleaseMemObject(ele_vtx);
    ocl.err = clReleaseMemObject(fac_vtx);

    //kernels
    ocl.err = clReleaseKernel(vtx_ini);
    ocl.err = clReleaseKernel(ele_ini);
    ocl.err = clReleaseKernel(fac_ini);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
