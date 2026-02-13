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
float det3(float3 a, float3 b, float3 c)
{
    return dot(a,cross(b,c));
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
    
    int vxl_idx = utl_idx(pos, vxl.ne);
    
//    printf("vxl %d [%d %d %d]\n", vxl_idx, pos.x, pos.y, pos.z);
    
    //centre
    float3 x = vxl.dx*(convert_float3(pos)+0.5f);
    vxl_dat[vxl_idx].xyz = x;
    
    //loop eles
    for (ulong ele_idx=0; ele_idx<tet.ne; ele_idx++)
    {
        float3 a = vtx_xyz[ele_vtx[ele_idx].x].xyz;
        float3 b = vtx_xyz[ele_vtx[ele_idx].y].xyz;
        float3 c = vtx_xyz[ele_vtx[ele_idx].z].xyz;
        float3 d = vtx_xyz[ele_vtx[ele_idx].w].xyz;
        
        //expand bottom row
        float detA = - det3(b,c,d) + det3(a,c,d) - det3(a,b,d) + det3(a,b,c);
//        printf("%d %lu %f\n", vxl_idx, ele_idx, detA);
        
        //bary coords
        float4 lam;
        
        lam.x = (- det3(b,c,d) + det3(x,c,d) - det3(x,b,d) + det3(x,b,c))/detA;
        lam.y = (- det3(x,c,d) + det3(a,c,d) - det3(a,x,d) + det3(a,x,c))/detA;
        lam.z = (- det3(b,x,d) + det3(a,x,d) - det3(a,b,d) + det3(a,b,x))/detA;
        lam.w = (- det3(b,c,x) + det3(a,c,x) - det3(a,b,x) + det3(a,b,c))/detA;
        
        
//        printf("%d %d a [%f %f %f]\n", vxl_idx, ele_idx, a.x, a.y, a.z);
//        printf("%d %d b [%f %f %f]\n", vxl_idx, ele_idx, b.x, b.y, b.z);
//        printf("%d %d c [%f %f %f]\n", vxl_idx, ele_idx, c.x, c.y, c.z);
//        printf("%d %d d [%f %f %f]\n", vxl_idx, ele_idx, d.x, d.y, d.z);
        
//        printf("%d %lu [%f %f %f %f]\n", vxl_idx, ele_idx, lam.x, lam.y, lam.z, lam.w);
        
        vxl_dat[vxl_idx].w += all(lam >= 0e0f);
    }
    
//    printf("%d [%f %f %f %f]\n", vxl_idx, vxl_dat[vxl_idx].x, vxl_dat[vxl_idx].y, vxl_dat[vxl_idx].z, vxl_dat[vxl_idx].w);

    return;
}



