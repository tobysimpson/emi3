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

constant float2 c1      = {0.5f,0.1f};
constant float2 c2      = {1.0f,1.0f};

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
                    global float            *gg,
                    global float2           *uu)
{
    int3 vxl_pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    int  vxl_idx = utl_idx(vxl_pos, vxl.ne);
    
    gg[vxl_idx] = vxl_pos.x >= vxl.ne.x/2;
    
    //init
//    uu[vxl_idx].x = convert_float(vxl_pos.x >= vxl.ne.x/2);
//    uu[vxl_idx].y = convert_float(vxl_pos.y >= vxl.ne.y/2);

    return;
}


//ee
kernel void vxl_ee1(const  struct vxl_obj    vxl,
                    global float            *gg,
                    global float2           *uu)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ne);
    
    float2 s = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = vxl_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ne);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ne);
        
        if(adj_bnd)
        {
            float2 dg = gg[adj_idx] - gg[vxl_idx];
            float2 du = uu[adj_idx] - uu[vxl_idx];
            
            s +=  c1*du + c2*(du - dg);
        }
    }
    
    //constants
    float2 alp = vxl.dt*vxl.rdx2;
    
    //ee
    uu[vxl_idx] += alp*s;

    return;
}


//ie jacobi
kernel void vxl_jac(const  struct vxl_obj    vxl,
                    global float            *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ne);
    
    float2 s = 0.0f;
    float2 d = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = vxl_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ne);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ne);
        
        if(adj_bnd)
        {
            d -= 1e0f;
            s += uu[adj_idx];
        }
    }
    
    //constants
    float2 alp = c1*vxl.dt*vxl.rdx2;
    
    //ie
    uu[vxl_idx] = (bb[vxl_idx] + alp*s)/(1e0f - alp*d);

    return;
}







/*
 ================================================
 ion implicit euler (I-alp*A)uˆ(t+1) = uˆ(t)
 ================================================
 */


/*
 
 //residual euler
 kernel void vxl_res1(const  struct msh_obj   msh,
                      global float            *uu,
                      global float            *bb,
                      global float            *rr,
                      global float4           *gg)
 {
     int3    vxl_pos = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
     int     vxl_idx = utl_idx1(vxl_pos, msh.ne);
     
     heart
     if(gg[vxl_idx].w<=0e0f)
     {
         float u = uu[vxl_idx];
         
         float s = 0.0f;
         float d = 0.0f;
         
         stencil
         for(int i=0; i<6; i++)
         {
             int3    adj_pos = vxl_pos + off_fac[i];
             int     adj_idx = utl_idx1(adj_pos, msh.ne);
             int     adj_bnd = utl_bnd1(adj_pos, msh.ne)*(gg[adj_idx].w<=0e0f);
             
             if(adj_bnd)
             {
                 d -= 1e0f;
                 s += uu[adj_idx];
             }
         }
         
         constants
         float alp = MD_SIG_H*msh.dt*msh.rdx2;
         
         lhs
         float Au = u - alp*(s + d*u);
         
         res
         rr[vxl_idx] = bb[vxl_idx] - Au;
     }
         
     return;
 }

 
 */

/*
 
 //jacobi euler
 kernel void vxl_jac1(const  struct msh_obj   msh,
                      global float            *uu,
                      global float            *bb,
                      global float4           *gg)
 {
     int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
     int   vxl_idx  = utl_idx1(vxl_pos, msh.ne);
     
     //heart
     if(gg[vxl_idx].w<=0e0f)
     {
         float s = 0.0f;
         float d = 0.0f;
         
         //stencil
         for(int i=0; i<6; i++)
         {
             int3    adj_pos = vxl_pos + off_fac[i];
             int     adj_idx = utl_idx1(adj_pos, msh.ne);
             int     adj_bnd = utl_bnd1(adj_pos, msh.ne)*(gg[adj_idx].w<=0e0f);
             
             if(adj_bnd)
             {
                 d -= 1e0f;
                 s += uu[adj_idx];
             }
         }
         
         //constants
         float alp = MD_SIG_H*msh.dt*msh.rdx2;
         
         //ie
         uu[vxl_idx] = (bb[vxl_idx] + alp*s)/(1e0f - alp*d);
     }
     
     return;
 }

 
 */

