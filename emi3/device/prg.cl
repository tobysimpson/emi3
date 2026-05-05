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

//passive conductivity
constant float2 cc1[9] = {{1.0f,1.0f},{1.0f,0.0f},{1.0f,0.0f},
                          {1.0f,0.0f},{1.0f,1.0f},{0.0f,0.0f},
                          {1.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f}};

//pump conductivity
constant float2 pp1[9] = {{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},
                          {0.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f},
                          {0.0f,0.0f},{1.0f,1.0f},{0.0f,0.0f}};

//pump level
constant float2 pp2[9] = {{+0.0f,+0.0f},{+0.0f,+0.0f},{+0.0f,+0.0f},
                          {+0.0f,+0.0f},{+0.0f,+0.0f},{+1.0f,-1.0f},
                          {+0.0f,+0.0f},{-1.0f,+1.0f},{+0.0f,+0.0f}};

constant float2 bb1[9] = {{0.0f,0.1f},{1.0f,1.1f},{2.0f,2.1f},
                          {3.0f,3.1f},{4.0f,4.1f},{5.0f,5.1f},
                          {6.0f,6.1f},{7.0f,7.1f},{8.0f,8.1f}};



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


struct vxl_obj
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
 kernel
 =============================
 */

kernel void vxl_ini(const  struct vxl_obj    vxl,
                    global int              *gg,
                    global float2           *uu)
{
    int3 vxl_pos = (int3){get_global_id(0),get_global_id(1),get_global_id(2)};
    int  vxl_idx = utl_idx(vxl_pos, vxl.ele.dim);
    
    //tags
    gg[vxl_idx] = (vxl_pos.x >= vxl.ele.dim.x/2)*((vxl_pos.y >= vxl.ele.dim.y/2)+1);
    
    //init
//    uu[vxl_idx] = vxl_pos.z == 0;
    
    return;
}


/*
 =============================
 ee pump/diff
 =============================
 */

//ee
kernel void vxl_exp(const  struct vxl_obj    vxl,
                    global int              *gg,
                    global float2           *uu,
                    global float2           *bb)
{
    int3  vxl_pos  = (int3){get_global_id(0), get_global_id(1), get_global_id(2)};
    int   vxl_idx  = utl_idx(vxl_pos, vxl.ele.dim);

//    printf("%d [%v3d]\n", vxl_idx, vxl_pos);
    
    float2 s = 0.0f;
    
    //stencil
    for(int j=0; j<6; j++)
    {
        int3    adj_pos = vxl_pos + off[j];
        int     adj_idx = utl_idx(adj_pos, vxl.ele.dim);
        int     adj_bnd = utl_bnd(adj_pos, vxl.ele.dim);
        
        
        
        if(adj_bnd)
        {
            int edg_typ = utl_nxn(gg[vxl_idx], gg[adj_idx], 3);
            
            float2 b = bb1[edg_typ];
            
//            printf("%d [%v3d] %d [%v3d] %d %d %f %d [%f,%f]\n", vxl_idx, vxl_pos, adj_idx, adj_pos, gg[vxl_idx], gg[adj_idx], aa[gg[vxl_idx]][gg[adj_idx]], edg_typ, b.x, b.y);
//            printf("%d [%v3d] %d [%v3d] (%d,%d) %d %f %f\n", vxl_idx, vxl_pos, adj_idx, adj_pos, gg[vxl_idx], gg[adj_idx], edg_typ, bb[edg_typ].x, b.x);
                printf("%d %f\n", edg_typ, b.y);
            
//            float2 dg = c3*(gg[adj_idx] - gg[vxl_idx]);     //geometry (size and direction)
//            float2 du = uu[adj_idx] - uu[vxl_idx];
//
//            s +=  c1*du + c2*(du - dg);                     //diffusion, pump conductivity
            
            s += b.x;
        }
    }

    //ee
    uu[vxl_idx] += s;

    return;
}



/*
 =============================
 ie pump/diff works
 =============================
 */

/*
 

//ie rhs
kernel void vxl_rhs(const  struct vxl_obj    vxl,
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
kernel void vxl_jac(const  struct vxl_obj    vxl,
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
 


