//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>

#include "read.h"
#include "write.h"

//working with mesh
int main(int argc, const char * argv[])
{
    printf("hello\n");
    printf("%s\n", ROOT_READ);
    
    write_xmf(0);
    
    
    //memory
    float   *vtx_xyz;
    int     *ele_vtx;
    int     *fac_vtx;
    int     *ele_fac;
    
    //dims
    long ne, nv, nf;
    
    //read
    nv = read_flt("vtx_xyz.raw", &vtx_xyz, 3);
    ne = read_int("ele_vtx.raw", &ele_vtx, 4);
    nf = read_int("fac_vtx.raw", &fac_vtx, 3);
    ne = read_int("ele_fac.raw", &ele_fac, 4);
    
    printf("vtx_xyz %ld\n",nv);
    for (int i=0; i<10; i++)
    {
        float*  xyz = &vtx_xyz[i*3];
        printf("%e %e %e\n", xyz[0], xyz[1], xyz[2]);
    }
    
    printf("ele_vtx %ld\n",ne);
    for (int i=0; i<10; i++)
    {
        int*  vtx = &ele_vtx[i*4];
        printf("%5d %5d %5d %5d\n", vtx[0], vtx[1], vtx[2], vtx[3]);
    }
    
    printf("fac_vtx %ld\n",nf);
    for (int i=0; i<10; i++)
    {
        int*  vtx = &fac_vtx[i*3];
        printf("%5d %5d %5d\n", vtx[0], vtx[1], vtx[2]);
    }
    
    printf("ele_fac %ld\n",ne);
    for (int i=0; i<10; i++)
    {
        int*  fac = &ele_fac[i*4];
        printf("%5d %5d %5d %5d\n", fac[0], fac[1], fac[2], fac[3]);
    }
    
    
    write_int("test1.raw", ele_fac, 10, 4);
    write_flt("test2.raw", vtx_xyz, 10, 4);


//    int n = 4;
//    
////    int *aa = malloc(10*4*sizeof(int));
//    int (*aa)[n] = malloc(sizeof(int[n][4]));
//    
//    
//    for (int i=0; i<n; i++)
//    {
//        for (int j=0; j<4; j++)
//        {
//            aa[i][j] = i*4+j;
//        }
//    }
//    
//    for (int i=0; i<n; i++)
//    {
//        printf("%2d %2d %2d %2d\n", aa[i][0], aa[i][1], aa[i][2], aa[i][3]);
//    }
//    
//    free(aa);

    /*
     ===============
     logic
     ===============
     */


    
//    union int4 *ele_fac = malloc(ne*sizeof(union int4));
//    
//    //ele_fac
//    for (int e1=0; e1<ne; e1++)
//    {
//        int i = 0;
//        
//        for (int f1=0; f1<nf; f1++)
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
//
//                ele_fac[e1].v[i] = f1;
//                
//                i += 1;
//            }
//            if(i==4)
//            {
//                break;
//            }
//            
//        }
//        printf("%6d %6d %6d %6d %6d\n", e1, ele_fac[e1].v[0], ele_fac[e1].v[1], ele_fac[e1].v[2], ele_fac[e1].v[3]);
//    }
//    
//    write_int4("ele_fac.raw", ele_fac, ne);
    
    
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
//    free(vtx_xyz);
//    free(ele_vtx);
//    free(fac_vtx);
//    free(ele_fac);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
