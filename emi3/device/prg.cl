//
//  prg.cl
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//


#include "sdf.h"

/*
 =============================
 const
 =============================
 */

//stencil
constant int3   off[6]  = {{-1,+0,+0},{+1,+0,+0},{+0,-1,+0},{+0,+1,+0},{+0,+0,-1},{+0,+0,+1}};

//diffusion rate
constant float2 cc1[4] = {{2.0f,2.0f},{0.0f,0.0f},
                          {0.0f,0.0f},{2.0f,2.0f}};

//gate rate
constant float2 gg1[4] = {{0.0f,0.0f},{2.0f,1.0f},
                          {2.0f,1.0f},{0.0f,0.0f}};

//pump rate
constant float2 pp1[4] = {{0.0f,0.0f},{0.3f,0.2f},
                          {0.3f,0.2f},{0.0f,0.0f}};

//pump equilibrium
constant float2 ee1[4] = {{+0.0f,+0.0f},{-1.0f,+1.0f},
                          {+1.0f,-1.0f},{+0.0f,+0.0f}};



/*
 =============================
 struct
 =============================
 */

struct dim_obj
{
    int3    dim;
    int     tot;
    ulong   sz[3];
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

//row-major nxn
int utl_nxn(int i, int j, int n)
{
    return n*i + j;
}

/*
 =============================
 membrane
 =============================
 */

//gate logistic shifted/scaled
float fn_g(float v)
{
    return pow(1e0f+exp((0.1f-v)/0.015f),-1);
}

/*
 =============================
 kernel
 =============================
 */

kernel void ele_ini(const  struct msh_obj   msh,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3 ele_pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    int  ele_idx = utl_idx(ele_pos, msh.ele.dim);
    
    int     g = 0;
    float2  u = 0.0f;
    float2  b = 0.0f;
    
    //sdf
//    float3  x = convert_float3(ele_pos);
//    float3  c = convert_float3(msh.ele.dim/2) - 0.5f;
//    float   r = convert_float(msh.ele.dim.x/3);
//    g = sdf_sph(x, c, r); // <= 0.0f;
    
    
    //geom
    g = (ele_pos.x >= msh.ele.dim.x/2);
    
    u.x = 0.5f - g;
    u.y = g - 0.5f;
    
    //write
    gg[ele_idx] = g;
    uu[ele_idx] = u;
    bb[ele_idx] = b;
    
    return;
}


/*
 =============================
 ee
 =============================
 */

//ee
kernel void ele_exp(const  struct msh_obj   msh,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb,
                    int                      t)
{
    int3  ele_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   ele_idx  = utl_idx(ele_pos, msh.ele.dim);

//    printf("%d [%v3d]\n", ele_idx, ele_pos);
    
    //read
    int     g = gg[ele_idx];
    float2  u = uu[ele_idx];
    
    //sum
    float2 s = 0.0f;
    
    //stencil
    for(int j=0; j<6; j++)
    {
        int3    adj_pos = ele_pos + off[j];
        int     adj_idx = utl_idx(adj_pos, msh.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, msh.ele.dim);
        
        //zero neumann
        if(adj_bnd)
        {
            //edge lookup 2x2
            int edg_typ = utl_nxn(gg[ele_idx], gg[adj_idx], 2);
            
            //constants
            float2 c1 = cc1[edg_typ];
            float2 g1 = gg1[edg_typ];
            float2 p1 = pp1[edg_typ];
            float2 e1 = ee1[edg_typ];
            
            //grad
            int     dg = gg[adj_idx] - g;
            float2  du = uu[adj_idx] - u;
            
            //voltage (wrt membrane)
            float v = dg*(du.x + du.y);
            
            
            //stim
            if((t>10)&&(t<20)&&dg!=0&&(ele_pos.z==0))
            {
                v = 0.2f;
            }
            
            if(dg==1&&all(ele_pos==(int3){3,0,0}))
//            if(dg==1)
            {
                printf("%03d [%v3d] %+d %+f\n",t,ele_pos,dg,v);
            }
            
            //flux
//            s += c1*du;
//            s += c1*du + p1*(du - e1);
//            s += c1*du + g1*fn_g(v)*du;
            s += c1*du + p1*(du - e1) + g1*fn_g(v)*du;
        }
    }
    
    //scale
    float alp = msh.dt;
    
    //ee
    uu[ele_idx] += alp*s;

    return;
}


/*
 =============================
 ie
 =============================
 */

//ie jacobi
kernel void ele_jac(const  struct msh_obj   msh,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  ele_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   ele_idx  = utl_idx(ele_pos, msh.ele.dim);
    
//    printf("%02d [%v3d]\n", ele_idx, ele_pos);
    
    //diag, off-diag
    float2 d = 0.0f;
    float2 s = 0.0f;
    
    //read
//    int     ele_geo = gg[ele_idx];
    float2  ele_u   = uu[ele_idx];
    
    //stencil
    for(int j=0; j<6; j++)
    {
        int3    adj_pos = ele_pos + off[j];
        int     adj_idx = utl_idx(adj_pos, msh.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, msh.ele.dim);
        
        //zero-neumann
        if(adj_bnd)
        {
            //read
//            int     adj_geo = gg[adj_idx];
            float2  adj_u   = uu[adj_idx];
            
            //edge lookup 2x2
//            int edg_typ = utl_nxn(ele_geo, adj_geo, 2);
            
            //constants
//            float2 c1 = cc1[edg_typ];
//            float2 g1 = gg1[edg_typ];
//            float2 p1 = pp1[edg_typ];
//            float2 e1 = ee1[edg_typ];
            
            //grad
//            int     dg = adj_geo - ele_geo;
//            float2  du = adj_u - ele_u;
            
            //voltage (wrt membrane)
//            float v = dg*(du.x + du.y);
            
            //conduct
            float2 c = 1.0f;
//            float2 c = c1 + p1 + g1*fn_g(v);
            
            //diag, off-diag
            d -= c;
            s += c*adj_u;
        }
    }
    
    //constants
    float alp = msh.dt*msh.rdx2;
    
    //ee
//    uu[ele_idx] += alp*(s + d*ele_u);
    
    //ie
    uu[ele_idx] = (bb[ele_idx] + alp*s)/(1e0f - alp*d);



    return;
}



/*
 =============================
 ie pump/diff works
 =============================
 */

/*
 

//ie rhs
kernel void vxl_rhs(const  struct msh_obj    vxl,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  ele_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   ele_idx  = utl_idx(ele_pos, vxl.ele.dim);
    
    float2 s = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = ele_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ele.dim);
        
        if(adj_bnd)
        {
            int cnd_idx = gg[ele_idx]*3 + gg[adj_idx];      //lookup
            
            float2 p1 = pp1[cnd_idx];                       //pump cond
            float2 p2 = pp2[cnd_idx];                       //pump level
            
            s += p1*p2;
        }
    }
    
    //constants
    float2 alp = vxl.dt;    //no dx pot diff. not grad
    
    //write
    bb[ele_idx] = uu[ele_idx] + alp*s;

    return;
}



//ie jacobi
kernel void vxl_jac(const  struct msh_obj    vxl,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  ele_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   ele_idx  = utl_idx(ele_pos, vxl.ele.dim);
    
    float2 s = 0.0f;
    float2 d = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = ele_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ele.dim);
        
        if(adj_bnd)
        {
            int cnd_idx = gg[ele_idx]*3 + gg[adj_idx];      //lookup (?? needs to be clearer)
            
            float2 c1 = cc1[cnd_idx];                        //passive conductivity
            float2 p1 = pp1[cnd_idx];                        //pump conductivity
            
            d -= (c1+p1);
            s += (c1+p1)*uu[adj_idx];
        }
    }
    
    //constants
    float2 alp = vxl.dt*vxl.rdx2;
    
    //ie
    uu[ele_idx] = (bb[ele_idx] + alp*s)/(1e0f - alp*d);

    return;
}

 */
 


