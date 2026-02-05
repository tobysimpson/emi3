//
//  msh.h
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//

#ifndef msh_h
#define msh_h



//    //memory
//    float   *vtx_xyz;
//    int     *ele_vtx;
//    int     *fac_vtx;
//    int     *ele_fac;
//    int     *ele_ele;
//    int     *ele_msk;



////dims
//long ne, nv, nf;
//
//long n = file_size("vtx_xyz.raw");
//
//printf("n=%ld\n",n);


//    nv = read_flt("vtx_xyz.raw", &vtx_xyz, 3);


//read
//    nv = read_flt("vtx_xyz.raw", &vtx_xyz, 3);
//    ne = read_int("ele_vtx.raw", &ele_vtx, 4);
//    nf = read_int("fac_vtx.raw", &fac_vtx, 3);
//    ne = read_int("ele_fac.raw", &ele_fac, 4);
//    ne = read_int("ele_ele.raw", &ele_ele, 4);
//    ne = read_int("ele_msk.raw", &ele_msk, 4);

//    printf("vtx_xyz %ld\n",nv);
//    for (int i=0; i<10; i+=3)
//    {
//        float*  xyz = &vtx_xyz[i];
//        printf("%e %e %e\n", xyz[0], xyz[1], xyz[2]);
//    }
//
//    printf("ele_vtx %ld\n",ne);
//    for (int i=0; i<10; i+=4)
//    {
//        int*  vtx = &ele_vtx[i];
//        printf("%6d %6d %5d %6d\n", vtx[0], vtx[1], vtx[2], vtx[3]);
//    }
//
//    printf("fac_vtx %ld\n",nf);
//    for (int i=0; i<10; i+=3)
//    {
//        int*  vtx = &fac_vtx[i];
//        printf("%6d %6d %6d\n", vtx[0], vtx[1], vtx[2]);
//    }
//
//    printf("ele_fac %ld\n",ne);
//    for (int i=0; i<10; i+=4)
//    {
//        int*  fac = &ele_fac[i];
//        printf("%6d %6d %6d %6d\n", fac[0], fac[1], fac[2], fac[3]);
//    }
//
//    printf("ele_ele %ld\n",ne);
//    for (int i=0; i<10; i+=4)
//    {
//        int*  ele = &ele_ele[i];
//        printf("%6d %6d %6d %6d\n", ele[0], ele[1], ele[2], ele[3]);
//    }




//    write_int("test1.raw", ele_fac, 10, 4);
//    write_flt("test2.raw", vtx_xyz, 10, 4);


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




//    int *ele_ele = malloc(ne*4*sizeof(int));
//    int *ele_msk = malloc(ne*4*sizeof(int));
//
//    //rows e1
//    for (int e1=0; e1<ne; e1++)
//    {
//        //cols e1
//        for (int j1=0; j1<4; j1++)
//        {
//            int e1_idx = e1*4+j1;
//
//            //init
//            ele_msk[e1_idx] = 0;
//
//            //rows e2
//            for (int e2=0; e2<ne; e2++)
//            {
//
//                if(e1!=e2)
//                {
//                    //cols e2
//                    for (int j2=0; j2<4; j2++)
//                    {
//                        int e2_idx = e2*4+j2;
//
//                        if(ele_fac[e1_idx]==ele_fac[e2_idx])
//                        {
//                            ele_ele[e1_idx] = e2;
//                            ele_msk[e1_idx] += 1;
//                        }
//                    }
//                }
//            }
//        }
////        printf("%6d %6d %6d %6d %d%d%d%d\n",ele_ele[e1+0], ele_ele[e1+1], ele_ele[e1+2], ele_ele[e1+3], ele_msk[e1+0], ele_msk[e1+1], ele_msk[e1+2], ele_msk[e1+3]);
//
//        if(ele_msk[e1+0]+ele_msk[e1+1]+ele_msk[e1+2]+ele_msk[e1+3] != 4)
//        {
////            printf("%6d %d%d%d%d\n", e1, ele_msk[e1+0], ele_msk[e1+1], ele_msk[e1+2], ele_msk[e1+3]);
//        }
//    }
//
//    write_int("ele_ele.raw", ele_ele, ne, 4);
//    write_int("ele_msk.raw", ele_msk, ne, 4);
    
    
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


////free
//free(vtx_xyz);
//free(ele_vtx);
//free(fac_vtx);
//free(ele_fac);

#endif /* msh_h */
