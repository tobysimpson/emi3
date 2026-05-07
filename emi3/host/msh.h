//
//  msh.h
//  emi3
//
//  Created by Toby Simpson on 12.02.2026.
//

#ifndef msh_h
#define msh_h


struct dim_obj
{
    cl_int3     dim;
    int         tot;
    size_t      sz[3];
};


struct msh_obj
{
    int     nt;
    float   dt;

    float   dx;
    float   rdx;
    float   rdx2;
    
    struct  dim_obj ele;
    struct  dim_obj vtx;
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
void msh_ini(struct msh_obj *msh)
{
    msh->vtx.dim.x = msh->ele.dim.x + 1;
    msh->vtx.dim.y = msh->ele.dim.y + 1;
    msh->vtx.dim.z = msh->ele.dim.z + 1;
    
    dim_ini(&msh->ele);
    dim_ini(&msh->vtx);
    
    msh->rdx  = powf(msh->dx, -1);
    msh->rdx2 = powf(msh->dx, -2);

    printf("dx %f\n", msh->dx);
    printf("dt %f\n", msh->dt);
    printf("ele [%d %d %d] %d\n", msh->ele.dim.x, msh->ele.dim.y, msh->ele.dim.z, msh->ele.tot);
//    printf("vtx [%d %d %d] %d\n", vxl->vtx.dim.x, vxl->vtx.dim.y, vxl->vtx.dim.z, vxl->vtx.tot);

    return;
}


#endif /* msh_h */
