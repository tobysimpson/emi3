//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>

#include "vec.h"
#include "read.h"
#include "write.h"

//working with mesh
int main(int argc, const char * argv[])
{
    printf("hello\n");
    printf("%s\n", ROOT_READ);
    
    write_xmf(0);
    
    
    //file
    FILE* file_ptr;
    char file_path[250];
    
    //memory
    struct flt3 *vtx_xyz;
    struct int4 *ele_vtx;
    struct int3 *fac_vtx;
    
    /*
     ===============
     vtx_xyz
     ===============
     */
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, "vtx_xyz.raw");
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long nv = flen(file_ptr)/sizeof(struct flt3);
    vtx_xyz = malloc(nv*sizeof(struct flt3));
    
    //read
    fread(vtx_xyz, sizeof(struct flt3), nv, file_ptr);
    fclose(file_ptr);
    
    /*
     ===============
     ele_vtx
     ===============
     */
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, "ele_vtx.raw");
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long ne = flen(file_ptr)/sizeof(struct int4);
    ele_vtx = malloc(ne*sizeof(struct int4));
    
    //read
    fread(ele_vtx, sizeof(struct int4), ne, file_ptr);
    fclose(file_ptr);
    
    /*
     ===============
     fac_vtx
     ===============
     */
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, "fac_vtx.raw");
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long nf = flen(file_ptr)/sizeof(struct int3);
    fac_vtx = malloc(ne*sizeof(struct int3));
    
    //read
    fread(fac_vtx, sizeof(struct int3), nf, file_ptr);
    fclose(file_ptr);
    
    //disp
    printf("%ld %ld %ld\n",ne,nv,nf);
    
    /*
     ===============
     debug
     ===============
     */
    
    
    //vtx
    for (int i = 0; i < 10; i++)
    {
        printf("%e %e %e\n", vtx_xyz[i].x, vtx_xyz[i].y, vtx_xyz[i].z);
    }
    
    //ele
    for (int i = 0; i < 10; i++)
    {
        printf("%6d %6d %6d %6d\n", ele_vtx[i].x, ele_vtx[i].y, ele_vtx[i].z, ele_vtx[i].w);
    }
    
    //fac
    for (int i = 0; i < 10; i++)
    {
        printf("%6d %6d %6d\n", fac_vtx[i].x, fac_vtx[i].y, fac_vtx[i].z);
    }
    
    
    /*
     ===============
     logic
     ===============
     */
    
    
    //fac-ele
    for (int f1 = 0; f1 <10; f1++)
    {
        for (int e1 = 0; e1 < ne; e1++)
        {
            int c = 0;
            
            c += (fac_vtx[f1].x == ele_vtx[e1].x);
            c += (fac_vtx[f1].x == ele_vtx[e1].y);
            c += (fac_vtx[f1].x == ele_vtx[e1].z);
            c += (fac_vtx[f1].x == ele_vtx[e1].w);
            
            c += (fac_vtx[f1].y == ele_vtx[e1].x);
            c += (fac_vtx[f1].y == ele_vtx[e1].y);
            c += (fac_vtx[f1].y == ele_vtx[e1].z);
            c += (fac_vtx[f1].y == ele_vtx[e1].w);
            
            c += (fac_vtx[f1].z == ele_vtx[e1].x);
            c += (fac_vtx[f1].z == ele_vtx[e1].y);
            c += (fac_vtx[f1].z == ele_vtx[e1].z);
            c += (fac_vtx[f1].z == ele_vtx[e1].w);
            
            if(c==3)
            {
                printf("%6d %6d %d\n", f1, e1, c);
            }
        }
    }
    
//    int aa[3] = {1,0,3};
//    
//    for (int i = 0; i < 3; i++)
//    {
//        printf("%d\n", aa[i]);
//    }
    
    
    
    
    //    //ele-ele
    //    for (int e1 = 0; e1 < ne; e1++)
    //    {
    //        for (int e2 = 0; e2 < ne; e2++)
    //        {
    //            if(e1!=e2)
    //            {
    //                int c = 0;
    //
    //                c += (ele_vtx[e1].x == ele_vtx[e2].x);
    //                c += (ele_vtx[e1].x == ele_vtx[e2].y);
    //                c += (ele_vtx[e1].x == ele_vtx[e2].z);
    //                c += (ele_vtx[e1].x == ele_vtx[e2].w);
    //
    //                c += (ele_vtx[e1].y == ele_vtx[e2].x);
    //                c += (ele_vtx[e1].y == ele_vtx[e2].y);
    //                c += (ele_vtx[e1].y == ele_vtx[e2].z);
    //                c += (ele_vtx[e1].y == ele_vtx[e2].w);
    //
    //                c += (ele_vtx[e1].z == ele_vtx[e2].x);
    //                c += (ele_vtx[e1].z == ele_vtx[e2].y);
    //                c += (ele_vtx[e1].z == ele_vtx[e2].z);
    //                c += (ele_vtx[e1].z == ele_vtx[e2].w);
    //
    //                c += (ele_vtx[e1].w == ele_vtx[e2].x);
    //                c += (ele_vtx[e1].w == ele_vtx[e2].y);
    //                c += (ele_vtx[e1].w == ele_vtx[e2].z);
    //                c += (ele_vtx[e1].w == ele_vtx[e2].w);
    //
    //                if(c==3)
    //                {
    //                    printf("%6d %6d %d\n", e1, e2, c);
    //                }
    //            }
    //        }
    //    }
    
    
    //clean
    free(vtx_xyz);
    free(ele_vtx);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
