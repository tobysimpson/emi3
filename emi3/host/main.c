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
#include "tet.h"
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
    
    //dim
    int le = 4;
    
    //voxels
    struct vxl_obj vxl;
    vxl.dt = 1e0f;
    vxl.dx = powf(2e0f, -le);
    vxl.le = (cl_int3){le,le,le};
    vxl_ini(&vxl);
    
    //tets
    struct tet_obj tet;
    tet.nv = file_size("vtx_xyz.dat")/(3*sizeof(float));
    tet.ne = file_size("tet_vtx.dat")/(4*sizeof(int));

    printf("tet.nv %zu\n", tet.nv);
    printf("tet.ne %zu\n", tet.ne);

    //memory
    cl_mem vtx_xyz = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, tet.nv*sizeof(cl_float4),       NULL, &ocl.err);
    cl_mem ele_vtx = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, tet.ne*sizeof(cl_int4),         NULL, &ocl.err);
    cl_mem vxl_dat = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, vxl.ne_tot*sizeof(cl_float4),   NULL, &ocl.err);
    
    //read
    read_flt4(&ocl, "vtx_xyz.dat", &vtx_xyz, tet.nv, 3);
    read_int4(&ocl, "tet_vtx.dat", &ele_vtx, tet.ne, 4);
    
    //kernels
    cl_kernel vtx_ini = clCreateKernel(ocl.program, "vtx_ini", &ocl.err);
    cl_kernel ele_ini = clCreateKernel(ocl.program, "ele_ini", &ocl.err);
    cl_kernel vxl_ini = clCreateKernel(ocl.program, "vxl_ini", &ocl.err);
    cl_kernel vxl_tet = clCreateKernel(ocl.program, "vxl_tet", &ocl.err);

    //args
    ocl.err = clSetKernelArg(vtx_ini,  0, sizeof(cl_mem),(void*)&vtx_xyz);
    
    ocl.err = clSetKernelArg(ele_ini,  0, sizeof(cl_mem),(void*)&vtx_xyz);
    ocl.err = clSetKernelArg(ele_ini,  1, sizeof(cl_mem),(void*)&ele_vtx);
    
    ocl.err = clSetKernelArg(vxl_ini,  0, sizeof(struct vxl_obj),(void*)&vxl);
    ocl.err = clSetKernelArg(vxl_ini,  1, sizeof(cl_mem),(void*)&vxl_dat);
    
    ocl.err = clSetKernelArg(vxl_tet,  0, sizeof(struct vxl_obj),(void*)&vxl);
    ocl.err = clSetKernelArg(vxl_tet,  1, sizeof(struct tet_obj),(void*)&tet);
    ocl.err = clSetKernelArg(vxl_tet,  2, sizeof(cl_mem),(void*)&vtx_xyz);
    ocl.err = clSetKernelArg(vxl_tet,  3, sizeof(cl_mem),(void*)&ele_vtx);
    ocl.err = clSetKernelArg(vxl_tet,  4, sizeof(cl_mem),(void*)&vxl_dat);

    //run
//    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vtx_ini, 1, NULL, &tet.nv, NULL, 0, NULL, &ocl.event);
//    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, ele_ini, 1, NULL, &tet.ne, NULL, 0, NULL, &ocl.event);
//    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_ini, 3, NULL, (size_t*)&vxl.ne_sz, NULL, 0, NULL, &ocl.event);
    ocl.err = clEnqueueNDRangeKernel(ocl.command_queue, vxl_tet, 3, NULL, (size_t*)&vxl.ne_sz, NULL, 0, NULL, &ocl.event);



    /*
     =============================
     write
     =============================
     */
    
    //write
    write_xmf(&tet, &vxl, 0);
    write_flt4(&ocl, "vxl_dat.dat", &vxl_dat, vxl.ne_tot, 4);


    
    /*
     =============================
     final
     =============================
     */
    
    //memory
    ocl.err = clReleaseMemObject(vtx_xyz);
    ocl.err = clReleaseMemObject(ele_vtx);
    ocl.err = clReleaseMemObject(vxl_dat);

    //kernels
    ocl.err = clReleaseKernel(vtx_ini);
    ocl.err = clReleaseKernel(ele_ini);
    ocl.err = clReleaseKernel(vxl_ini);
    ocl.err = clReleaseKernel(vxl_tet);
    
    //final
    ocl_fin(&ocl);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
