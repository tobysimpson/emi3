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
    cl_float    dx;
    cl_float3   x0;
    cl_float3   x1;
    
    cl_int3     ne;
    cl_int3     nv;
    
    cl_int      ne_tot;
    cl_int      nv_tot;
    
    size_t      nv_sz[3];
    size_t      ne_sz[3];
};



//init
void vxl_ini(struct vxl_obj *vxl)
{
    vxl->ne.x = ceilf((vxl->x1.x - vxl->x0.x)/vxl->dx);
    vxl->ne.y = ceilf((vxl->x1.y - vxl->x0.y)/vxl->dx);
    vxl->ne.z = ceilf((vxl->x1.z - vxl->x0.z)/vxl->dx);
    
    vxl->nv.x = vxl->ne.x+1;
    vxl->nv.y = vxl->ne.y+1;
    vxl->nv.z = vxl->ne.z+1;

    vxl->ne_tot = vxl->ne.x*vxl->ne.y*vxl->ne.z;
    vxl->nv_tot = vxl->nv.x*vxl->nv.y*vxl->nv.z;

    
    vxl->nv_sz[0]   = vxl->nv.x;
    vxl->nv_sz[1]   = vxl->nv.y;
    vxl->nv_sz[2]   = vxl->nv.z;
    
    vxl->ne_sz[0]   = vxl->ne.x;
    vxl->ne_sz[1]   = vxl->ne.y;
    vxl->ne_sz[2]   = vxl->ne.z;
    
    printf("vxl.dx %f\n", vxl->dx);
    printf("vxl.ne [%u,%u,%u]\n", vxl->ne.x, vxl->ne.y, vxl->ne.z);
    printf("vxl.ne_tot %3u\n", vxl->ne_tot);

    return;
}


#endif /* vxl_h */
