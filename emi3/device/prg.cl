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

//stencil
constant int3   off[6]  = {{-1,+0,+0},{+1,+0,+0},{+0,-1,+0},{+0,+1,+0},{+0,+0,-1},{+0,+0,+1}};

//diffusion rate
constant float2 cc1[4] = {{1.0f,1.0f},{0.0f,0.0f},
                          {0.0f,0.0f},{1.0f,1.0f}};

//pump rate
constant float2 pp1[4] = {{0.0f,0.0f},{0.3f,0.2f},
                          {0.3f,0.2f},{0.0f,0.0f}};

//pump equilibrium
constant float2 ee1[4] = {{+0.0f,+0.0f},{-1.0f,+1.0f},
                          {+1.0f,-1.0f},{+0.0f,+0.0f}};

//gate rate
constant float2 gg1[4] = {{0.0f,0.0f},{3.0f,1.0f},
                          {3.0f,1.0f},{0.0f,0.0f}};

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
    float dt;
    float dx;
    
    struct dim_obj ele;
    struct dim_obj vtx;

    float rdx;
    float rdx2;
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
    return pow(1e0f+exp((0.2f-v)/0.02f),-1e0f);
}

/*
 =============================
 kernel
 =============================
 */

kernel void vxl_ini(const  struct msh_obj   msh,
                    global int              *gg,
                    global float2           *uu)
{
    int3 vxl_pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    int  vxl_idx = utl_idx(vxl_pos, msh.ele.dim);
    
    int     g = 0;
    float2  u = 0.0f;
    
    //geom
    g = (vxl_pos.x >= msh.ele.dim.x/2);
    
    //init
    u.x = 0.5f - g;
    u.y = g - 0.5f;
    
    //stim
    int3 s0 = {0,0,0};
    int3 s1 = {1,0,0};


    if(all(vxl_pos==s0))
    {
        u.x = +0.3f;
    }
    
    if(all(vxl_pos==s1))
    {
        u.x = -0.3f;
    }
    
    //write
    gg[vxl_idx] = g;
    uu[vxl_idx] = u;
    
    return;
}


/*
 =============================
 ee pump/diff
 =============================
 */

//ee
kernel void vxl_exp(const  struct msh_obj    msh,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, msh.ele.dim);

//    printf("%d [%v3d]\n", vxl_idx, vxl_pos);
    
    float2 dp = 0.0f;
    float2 dg = 0.0f;
    float2 dc = 0.0f;
    
    //read
    float2 u = uu[vxl_idx];
    
    //stencil
    for(int j=0; j<6; j++)
    {
        int3    adj_pos = vxl_pos + off[j];
        int     adj_idx = utl_idx(adj_pos, msh.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, msh.ele.dim);
        
        //zero neumann
        if(adj_bnd)
        {
            //edge lookup 2x2
            int edg_typ = utl_nxn(gg[vxl_idx], gg[adj_idx], 2);
            
            //constants
            float2 c = cc1[edg_typ];
            float2 p = pp1[edg_typ];
            float2 e = ee1[edg_typ];
            float2 g = gg1[edg_typ];
            
            //grad
            float2 du = uu[adj_idx] - u;
            
            //voltage - directed wrt membrane
            float v = (gg[adj_idx] - gg[vxl_idx])*(du.x + du.y);
            
            if(vxl_idx==1&&adj_idx==0)
            {
                printf("%+f %+f %+f %+f %+f %+f\n",v,du.x,du.y,u.x,u.y,fn_g(v));
            }
            
            //flux
            dc += c*du;
            dp += p*(du - e);
            dg += g*fn_g(v)*du;
        }
    }
    
    //scale
    float alp = msh.dt*msh.rdx2;
    
    //ee
    uu[vxl_idx] += alp*(10.0f*dc + dp + dg);

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
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ele.dim);
    
    float2 s = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = vxl_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ele.dim);
        
        if(adj_bnd)
        {
            int cnd_idx = gg[vxl_idx]*3 + gg[adj_idx];      //lookup
            
            float2 p1 = pp1[cnd_idx];                       //pump cond
            float2 p2 = pp2[cnd_idx];                       //pump level
            
            s += p1*p2;
        }
    }
    
    //constants
    float2 alp = vxl.dt;    //no dx pot diff. not grad
    
    //write
    bb[vxl_idx] = uu[vxl_idx] + alp*s;

    return;
}



//ie jacobi
kernel void vxl_jac(const  struct msh_obj    vxl,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ele.dim);
    
    float2 s = 0.0f;
    float2 d = 0.0f;
    
    //stencil
    for(int i=0; i<6; i++)
    {
        int3    adj_pos = vxl_pos + off[i];
        int     adj_idx = utl_idx(adj_pos, vxl.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ele.dim);
        
        if(adj_bnd)
        {
            int cnd_idx = gg[vxl_idx]*3 + gg[adj_idx];      //lookup (?? needs to be clearer)
            
            float2 c1 = cc1[cnd_idx];                        //passive conductivity
            float2 p1 = pp1[cnd_idx];                        //pump conductivity
            
            d -= (c1+p1);
            s += (c1+p1)*uu[adj_idx];
        }
    }
    
    //constants
    float2 alp = vxl.dt*vxl.rdx2;
    
    //ie
    uu[vxl_idx] = (bb[vxl_idx] + alp*s)/(1e0f - alp*d);

    return;
}

 */
 


