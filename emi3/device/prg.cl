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
    int3    le;
    int3    ne;
    int3    nv;
    
    int     ne_tot;
    int     nv_tot;
    
    float    dt;
    float    dx;
    float    dx2;    //dx*dx
    float    rdx2;   //1/(dx*dx)
    
    ulong3   nv_sz;
    ulong3   ne_sz;
};

//object
struct tet_obj
{
    ulong   nv;
    ulong   ne;
};

/*
 =============================
 util
 =============================
 */

//global index
int utl_idx(int3 pos, int3 dim)
{
    return pos.x + dim.x*pos.y + dim.x*dim.y*pos.z;
}

/*
 =============================
 linear
 =============================
 */

//determinant 3x3
int det3(int3 pos, int3 dim)
{
    return pos.x + dim.x*pos.y + dim.x*dim.y*pos.z;
}


/*
 =============================
 kernel
 =============================
 */

kernel void vtx_ini(global float4 *vtx_xyz)
{
    int  idx = get_global_id(0);
    
    printf("%d %e %e %e %e\n", idx, vtx_xyz[idx].x, vtx_xyz[idx].y, vtx_xyz[idx].z, vtx_xyz[idx].w);
    
    return;
}

kernel void ele_ini(global float4   *vtx_xyz,
                    global int4     *ele_vtx)
{
    int  idx = get_global_id(0);

    printf("ele %d %d %d %d %d\n", idx, ele_vtx[idx].x, ele_vtx[idx].y, ele_vtx[idx].z, ele_vtx[idx].w);
    
    //verts
    printf("vtx %d %f %f %f\n", ele_vtx[idx].x, vtx_xyz[ele_vtx[idx].x].x, vtx_xyz[ele_vtx[idx].x].y, vtx_xyz[ele_vtx[idx].x].z);
    printf("vtx %d %f %f %f\n", ele_vtx[idx].y, vtx_xyz[ele_vtx[idx].y].x, vtx_xyz[ele_vtx[idx].y].y, vtx_xyz[ele_vtx[idx].y].z);
    printf("vtx %d %f %f %f\n", ele_vtx[idx].z, vtx_xyz[ele_vtx[idx].z].x, vtx_xyz[ele_vtx[idx].z].y, vtx_xyz[ele_vtx[idx].z].z);
    printf("vtx %d %f %f %f\n", ele_vtx[idx].w, vtx_xyz[ele_vtx[idx].w].x, vtx_xyz[ele_vtx[idx].w].y, vtx_xyz[ele_vtx[idx].w].z);

    return;
}

kernel void vxl_ini(const  struct  vxl_obj  vxl,
                    global float4           *vxl_dat)
{
    int3  pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    
    int idx = utl_idx(pos, vxl.ne);
    
    vxl_dat[idx] = convert_float4((int4){pos.x,pos.y,pos.z,idx});
    
    printf("%d [%d %d %d] %e \n", idx, pos.x, pos.y, pos.z, vxl_dat[idx].w);

    return;
}

kernel void vxl_tet(const  struct  vxl_obj  vxl,
                    const  struct  tet_obj  tet,
                    global float4           *vtx_xyz,
                    global int4             *ele_vtx,
                    global float4           *vxl_dat)
{
    int3  pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    
    int idx = utl_idx(pos, vxl.ne);
    
    printf("%d [%d %d %d %lu]\n", idx, pos.x, pos.y, pos.z, tet.nv);
    
    //centre
    float3 c = vxl.dx*(convert_float3(pos)+0.5f);
    
    //ele
    for (ulong i=0; i<tet.ne; i++)
    {
        //verts
        float3 vtx[4];
        
        vtx[0] = vtx_xyz[ele_vtx[i].x].xyz;
        vtx[1] = vtx_xyz[ele_vtx[i].y].xyz;
        vtx[2] = vtx_xyz[ele_vtx[i].z].xyz;
        vtx[3] = vtx_xyz[ele_vtx[i].w].xyz;
    }
    
    //centre
    vxl_dat[idx].xyz = c;
    
    printf("%d [%f %f %f %f]\n", idx, vxl_dat[idx].x, vxl_dat[idx].y, vxl_dat[idx].z, vxl_dat[idx].w);

    return;
}



