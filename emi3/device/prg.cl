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
constant float2 c       = {1.0f,1.0f};

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
    
    //init
    uu[vxl_idx].x = convert_float(vxl_pos.x >= vxl.ne.x/2);
    uu[vxl_idx].y = convert_float(vxl_pos.y >= vxl.ne.y/2);

    
//    uu[vxl_idx] = convert_float(vxl_pos.xy > vxl.ne.xy/2);
    
//    printf("ini %5d [%2d %2d %2d] %f %f\n", idx, pos.x, pos.y, pos.z, vxl_dat[idx].x, vxl_dat[idx].y);

    return;
}


//jacobi euler
kernel void vxl_jac(const  struct vxl_obj    vxl,
                    global float            *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ne);
    
    float2 s = 0.0f;
    float  d = 0.0f;
    
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
    float2 alp = c*vxl.dt*vxl.rdx2;
    
    //ie
    uu[vxl_idx] = (bb[vxl_idx] + alp*s)/(1e0f - alp*d);

    
    return;
}







//explicit euler
kernel void vxl_ion(const  struct vxl_obj    vxl,
                    global float            *gg,
                    global float2           *uu)
{
    int3 pos = {get_global_id(0), get_global_id(1), get_global_id(2)};
    int  idx = utl_idx(pos, vxl.ne);
     
    //sum
    float2  d = 0.0f;
    float2  s = 0.0f;

    //stencil
    for(int j=0; j<6; j++)
    {
        int3 posj = pos + off[j];
        int  bndj = utl_bnd(posj, vxl.ne);
        int  idxj = utl_idx(posj, vxl.ne);
        
        d += bndj;
        s += bndj*uu[idxj];
    }
    
    //explicit
    uu[idx] += vxl.dt*vxl.rdx2*c*(s - d*uu[idx]);
    
    //fwd
    //Au[idx] = vxl.rdx2*(6.0f*uu[idx] - s);

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

