//
//  prg.cl
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//



kernel void vtx_ini(global float4 *vtx_xyz)
{
    int  pos = get_global_id(0);
    
    printf("%e %e %e %e\n", vtx_xyz[pos].x, vtx_xyz[pos].y, vtx_xyz[pos].z, vtx_xyz[pos].w);
    
    return;
}

kernel void ele_ini(global int4 *ele_vtx)
{
    int  pos = get_global_id(0);
    
    printf("%8d %8d %8d %8d\n", ele_vtx[pos].x, ele_vtx[pos].y, ele_vtx[pos].z, ele_vtx[pos].w);
    
    return;
}

kernel void fac_ini(global int4 *fac_vtx)
{
    int  pos = get_global_id(0);
    
    printf("%8d %8d %8d %8d\n", fac_vtx[pos].x, fac_vtx[pos].y, fac_vtx[pos].z, fac_vtx[pos].w);
    
    return;
}




