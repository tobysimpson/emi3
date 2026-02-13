//
//  vxl.h
//  emi3
//
//  Created by Toby Simpson on 12.02.2026.
//

#ifndef vxl_h
#define vxl_h


//object
struct vxl_obj
{
    cl_int3    le;
    cl_int3    ne;
    cl_int3    nv;
    
    cl_int     ne_tot;
    cl_int     nv_tot;
    
    cl_float   dt;
    cl_float   dx;
    cl_float   dx2;    //dx*dx
    cl_float   rdx2;   //1/(dx*dx)
    
    size_t     nv_sz[3];
    size_t     ne_sz[3];
};



//init
void vxl_ini(struct vxl_obj *vxl)
{
    vxl->ne = (cl_int3){pow(2, vxl->le.x), pow(2, vxl->le.y), pow(2, vxl->le.z)};
    vxl->nv = (cl_int3){vxl->ne.x+1, vxl->ne.y+1, vxl->ne.z+1};
    
    vxl->ne_tot = vxl->ne.x*vxl->ne.y*vxl->ne.z;
    vxl->nv_tot = vxl->nv.x*vxl->nv.y*vxl->nv.z;
    
    vxl->dx2    = vxl->dx*vxl->dx;
    vxl->rdx2   = 1e0f/vxl->dx2;
    
    vxl->nv_sz[0]   = vxl->nv.x;
    vxl->nv_sz[1]   = vxl->nv.y;
    vxl->nv_sz[2]   = vxl->nv.z;
    
    vxl->ne_sz[0]   = vxl->ne.x;
    vxl->ne_sz[1]   = vxl->ne.y;
    vxl->ne_sz[2]   = vxl->ne.z;
    
    printf("vxl.dt %f\n", vxl->dt);
    printf("vxl.dx %f\n", vxl->dx);
    printf("vxl [%u,%u,%u] %3u\n", vxl->ne.x, vxl->ne.y, vxl->ne.z, vxl->ne_tot);

    return;
}


#endif /* vxl_h */
