//
//  prg.cl
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//



kernel void vtx_ini(global float4 *vtx_xyz,
                    global int4 *vtx_dat)
{
    int  pos = get_global_id(0);
    
//    vtx_dat[pos] = (int4){get_global_id(0), get_group_id(0), get_local_id(0), get_local_size(0)};
    
    vtx_dat[pos] = (int4){get_group_id(0),2,3,4};
    
    //printf("%8d %e %e %e %e\n", pos, vtx_xyz[pos].x, vtx_xyz[pos].y, vtx_xyz[pos].z, vtx_xyz[pos].w);
    
    return;
}

kernel void ele_ini(global int4 *ele_vtx,
                    global int4 *ele_dat)
{
    int  pos = get_global_id(0);
    
    //printf("%8d %8d %8d %8d %8d\n", pos, ele_vtx[pos].x, ele_vtx[pos].y, ele_vtx[pos].z, ele_vtx[pos].w);
    
    ele_dat[pos] = (int4){get_global_id(0), get_group_id(0), get_local_id(0), get_local_size(0)};
    
    return;
}

kernel void fac_ini(global int4 *fac_vtx)
{
    int  pos = get_global_id(0);
    
    printf("%8d %8d %8d %8d %8d\n", pos, fac_vtx[pos].x, fac_vtx[pos].y, fac_vtx[pos].z, fac_vtx[pos].w);
    
    return;
}




