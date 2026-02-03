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
    
    
    //memory
    struct flt3 *vtx_xyz;
    struct int4 *ele_vtx;
    struct int3 *fac_vtx;
    
    //read
    long nv = read_flt3("vtx_xyz.raw", &vtx_xyz);
    long ne = read_int4("ele_vtx.raw", &ele_vtx);
    long nf = read_int3("fac_vtx.raw", &fac_vtx);
    
    int n = 10;
    const int w = 4;
    
    union flt4 *aa = malloc(n*sizeof(union flt4));

    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<w; j++)
        {
            aa[i].v[j] = (i*w)+j;
        }
    }
    
    for (int i = 0; i<n; i++)
    {
        printf("%e %e %e %e\n", aa[i].x, aa[i].y, aa[i].z, aa[i].w);
    }
    
    free(aa);
    
    //    for (int i = 0; i < 10; i++)
    //    {
    //        printf("%e %e %e\n", vtx_xyz[i].x, vtx_xyz[i].y, vtx_xyz[i].z);
    //    }
    

    
    //disp
//    printf("%ld %ld %ld\n",ne,nv,nf);
//    
//    //vtx
//    for (int i = 0; i < 10; i++)
//    {
//        printf("%e %e %e\n", vtx_xyz[i].x, vtx_xyz[i].y, vtx_xyz[i].z);
//    }
//
//    //ele
//    for (int i = 0; i < 10; i++)
//    {
//        printf("%6d %6d %6d %6d\n", ele_vtx[i].x, ele_vtx[i].y, ele_vtx[i].z, ele_vtx[i].w);
//    }
//
//    //fac
//    for (int i = 0; i < 10; i++)
//    {
//        printf("%6d %6d %6d\n", fac_vtx[i].x, fac_vtx[i].y, fac_vtx[i].z);
//    }
    

    /*
     ===============
     logic
     ===============
     */

//    
//    //ele_adj
//    
//    struct int4 *ele_fac = malloc(ne*sizeof(struct int4));
//    
//    //ele-fac
//    for (int e1=0; e1<10; e1++)
//    {
//        for (int f1=0; f1<nf; f1++)
//        {
//            int i = 0;
//            
//            int c = 0;
//            
//            c += (fac_vtx[f1].x == ele_vtx[e1].x);
//            c += (fac_vtx[f1].x == ele_vtx[e1].y);
//            c += (fac_vtx[f1].x == ele_vtx[e1].z);
//            c += (fac_vtx[f1].x == ele_vtx[e1].w);
//            
//            c += (fac_vtx[f1].y == ele_vtx[e1].x);
//            c += (fac_vtx[f1].y == ele_vtx[e1].y);
//            c += (fac_vtx[f1].y == ele_vtx[e1].z);
//            c += (fac_vtx[f1].y == ele_vtx[e1].w);
//            
//            c += (fac_vtx[f1].z == ele_vtx[e1].x);
//            c += (fac_vtx[f1].z == ele_vtx[e1].y);
//            c += (fac_vtx[f1].z == ele_vtx[e1].z);
//            c += (fac_vtx[f1].z == ele_vtx[e1].w);
//            
//            if(c==3)
//            {
//                printf("%6d %6d %d\n", e1, f1, c);
//                
//                
//            }
//        }
//    }
    
    
//    //fac-ele
//    for (int f1 = 0; f1 <10; f1++)
//    {
//        for (int e1 = 0; e1 < ne; e1++)
//        {
//            int c = 0;
//            
//            c += (fac_vtx[f1].x == ele_vtx[e1].x);
//            c += (fac_vtx[f1].x == ele_vtx[e1].y);
//            c += (fac_vtx[f1].x == ele_vtx[e1].z);
//            c += (fac_vtx[f1].x == ele_vtx[e1].w);
//            
//            c += (fac_vtx[f1].y == ele_vtx[e1].x);
//            c += (fac_vtx[f1].y == ele_vtx[e1].y);
//            c += (fac_vtx[f1].y == ele_vtx[e1].z);
//            c += (fac_vtx[f1].y == ele_vtx[e1].w);
//            
//            c += (fac_vtx[f1].z == ele_vtx[e1].x);
//            c += (fac_vtx[f1].z == ele_vtx[e1].y);
//            c += (fac_vtx[f1].z == ele_vtx[e1].z);
//            c += (fac_vtx[f1].z == ele_vtx[e1].w);
//            
//            if(c==3)
//            {
//                printf("%6d %6d %d\n", f1, e1, c);
//            }
//        }
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
    free(fac_vtx);
//    free(ele_fac);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
