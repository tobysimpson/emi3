//
//  vxl.h
//  emi3
//
//  Created by Toby Simpson on 12.02.2026.
//

#ifndef vxl_h
#define vxl_h


struct dim_obj
{
    cl_int3     dim;
    int         tot;
    size_t      sz[3];
};


struct vxl_obj
{
    float dt;
    float dx;
    
    struct dim_obj ele;
    struct dim_obj vtx;

    float rdx;
    float rdx2;
};


//init
void dim_ini(struct dim_obj *obj)
{
    obj->tot = obj->dim.x*obj->dim.y*obj->dim.z;
    
    obj->sz[0] = obj->dim.x;
    obj->sz[1] = obj->dim.y;
    obj->sz[2] = obj->dim.z;

    return;
}

//init
void vxl_ini(struct vxl_obj *vxl)
{
    vxl->vtx.dim.x = vxl->ele.dim.x + 1;
    vxl->vtx.dim.y = vxl->ele.dim.y + 1;
    vxl->vtx.dim.z = vxl->ele.dim.z + 1;
    
    dim_ini(&vxl->ele);
    dim_ini(&vxl->vtx);
    
    vxl->rdx  = powf(vxl->dx, -1);
    vxl->rdx2 = powf(vxl->dx, -2);

    printf("dx %f dt %f\n", vxl->dx, vxl->dt);
    printf("ele [%d %d %d] %d\n", vxl->ele.dim.x, vxl->ele.dim.y, vxl->ele.dim.z, vxl->ele.tot);
//    printf("vtx [%d %d %d] %d\n", vxl->vtx.dim.x, vxl->vtx.dim.y, vxl->vtx.dim.z, vxl->vtx.tot);

    return;
}


#endif /* vxl_h */
