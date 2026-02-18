//
//  prg.cl
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//

/*
 =============================
 struct
 =============================
 */

//object
struct vxl_obj
{
    float dx;
    
    float3 x0;
    float3 x1;
    
    int3 ne;
    int3 nv;
    
    int ne_tot;
    int nv_tot;
    
    ulong ne_sz[3];
    ulong nv_sz[3];
};


/*
 =============================
 util
 =============================
 */

//global index
int utl_idx(int3 pos, int3 dim)
{
    return pos.x + dim.x*(pos.y + dim.y*pos.z);
}

/*
 =============================
 kernel
 =============================
 */

kernel void vxl_ini(const  struct  vxl_obj  vxl,
                    global float   *vxl_tag)
{
    int3  pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    
    int vxl_idx = utl_idx(pos, vxl.ne);
    
    printf("vxl %6d [%3d %3d %3d] %f \n", vxl_idx, pos.x, pos.y, pos.z, vxl_tag[vxl_idx]);

    return;
}
