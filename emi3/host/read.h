//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef io_h
#define io_h

#define ROOT_READ   "/Users/toby/Downloads"





void read_raw(void);
//fwrite(ptr1, sizeof(float), msh->nv_tot, file1);

//write raw
void read_raw(void)
{
    FILE* file1;
    char file1_name[250];
    
    
    sprintf(file1_name, "%s/xx.raw", ROOT_READ);
    
    //mem
//    float bb[5][2];

    
    //open
    file1 = fopen(file1_name,"rb");
    
    fseek(file1, 0, SEEK_END);
    long n = ftell(file1)/sizeof(struct flt3);
    fseek(file1, 0, SEEK_SET);
    printf("%ld\n", n);

    struct flt3 *bb = malloc(n*sizeof(struct flt3));
    
    fread(bb, sizeof(struct flt3), n, file1);
    fclose(file1);
    

    for (int i = 0; i < 100; i++)
    {
        printf("%e %e %e\n", bb[i].x, bb[i].y, bb[i].z);
    }

    free(bb);
    
    
    
    return;
}




#endif /* io_h */
