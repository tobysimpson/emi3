//
//  prg.cl
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//


/*
 =============================
 const
 =============================
 */

constant int3   off[6]  = {{-1,0,0},{+1,0,0},{0,-1,0},{0,+1,0},{0,0,-1},{0,0,+1}};
constant float2 c       = 5.0f;

/*
 =============================
 struct
 =============================
 */


//object
struct vxl_obj
{
    float dt;
    float dx;
    
    float3 x0;
    float3 x1;
    
    int3 ne;
    int3 nv;
    
    int ne_tot;
    int nv_tot;
    
    float rdx2;
    
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

//in-bounds
int utl_bnd(int3 pos, int3 dim)
{
    return all(pos>=0)&&all(pos<dim);
}

/*
 =============================
 kernel
 =============================
 */

kernel void vxl_ini(const  struct vxl_obj    vxl,
                    global float2           *vxl_dat,
                    global float            *vxl_tag)
{
    int3 pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    int  idx = utl_idx(pos, vxl.ne);
    
    //init
    vxl_dat[idx].xy = convert_float2(pos.xy > vxl.ne.xy/2);
    
//    printf("ini %5d [%2d %2d %2d] %f %f\n", idx, pos.x, pos.y, pos.z, vxl_dat[idx].x, vxl_dat[idx].y);

    return;
}


//forward Au = b
kernel void vxl_ion(const  struct vxl_obj    vxl,
                    global float2           *vxl_dat,
                    global float            *vxl_tag)
{
    int3 pos = {get_global_id(0), get_global_id(1), get_global_id(2)};
    int  idx = utl_idx(pos, vxl.ne);
     

    //sum
    float   d = 0.0f;
    float2  s = 0.0f;

    //stencil
    for(int j=0; j<6; j++)
    {
        int3 posj = pos + off[j];
        int  bndj = utl_bnd(posj, vxl.ne);
        int  idxj = utl_idx(posj, vxl.ne);
        
        d += bndj;
        
        s += bndj*vxl_dat[idxj];
    }
    
    vxl_dat[idx] += vxl.dt*vxl.rdx2*c*(s - d*vxl_dat[idx]);
    
    //fwd
    //Au[idx] = vxl.rdx2*(6.0f*uu[idx] - s);

    return;
}


/*
 ================================================
 ion implicit euler with fibres
 ================================================


//residual euler
kernel void ele_res1(const  struct msh_obj   msh,
                     global float4           *gg,
                     global float            *uu,
                     global float            *bb,
                     global float            *rr)
{
    int3    ele_pos = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int     ele_idx = utl_idx1(ele_pos, msh.ne);
    
    //heart
    if(gg[ele_idx].w<=0e0f)
    {
        float u = uu[ele_idx];
        
        float s = 0.0f;
        float d = 0.0f;
        
        //stencil
        for(int i=0; i<6; i++)
        {
            int3    adj_pos = ele_pos + off_fac[i];
            int     adj_idx = utl_idx1(adj_pos, msh.ne);
            int     adj_bnd = utl_bnd1(adj_pos, msh.ne)*(gg[adj_idx].w<=0e0f);
            
            if(adj_bnd)
            {
                //conductivity - interp fibre and dot
                float c = fabs(dot(0.5f*(gg[ele_idx] + gg[adj_idx]).xyz, convert_float3(off_fac[i])));
                
                d -= c;
                s += c*uu[adj_idx];
            }
        }
        
        //constants
        float alp = msh.dt*msh.rdx2;
        
        //lhs
        float Au = u - alp*(s + d*u);
        
        //res
        rr[ele_idx] = bb[ele_idx] - Au;
    }
        
    return;
}


//jacobi euler
kernel void ele_jac1(const  struct msh_obj   msh,
                     global float4           *gg,
                     global float            *uu,
                     global float            *bb)
{
    int3  ele_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   ele_idx  = utl_idx1(ele_pos, msh.ne);
    
    //heart
    if(gg[ele_idx].w<=0e0f)
    {
        float s = 0.0f;
        float d = 0.0f;
        
        //stencil
        for(int i=0; i<6; i++)
        {
            int3    adj_pos = ele_pos + off_fac[i];
            int     adj_idx = utl_idx1(adj_pos, msh.ne);
            int     adj_bnd = utl_bnd1(adj_pos, msh.ne)*(gg[adj_idx].w<=0e0f);
            
            if(adj_bnd)
            {
                //conductivity - interp fibre and dot
                float c = fabs(dot(0.5f*(gg[ele_idx] + gg[adj_idx]).xyz, convert_float3(off_fac[i])));
                
                d -= c;
                s += c*uu[adj_idx];
            }
        }
        
        //constants
        float alp = msh.dt*msh.rdx2;
        
        //ie
        uu[ele_idx] = (bb[ele_idx] + alp*s)/(1e0f - alp*d);
    }
    
    return;
}

*/
