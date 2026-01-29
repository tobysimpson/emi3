//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef io_h
#define io_h

#define ROOT_IO   "/Users/toby/Downloads"


struct flt3
{
    float x;
    float y;
    float z;
};


void read_raw(void);
//fwrite(ptr1, sizeof(float), msh->nv_tot, file1);

//write raw
void read_raw(void)
{
    FILE* file1;
    char file1_name[250];
    
    
    sprintf(file1_name, "%s/bb.raw", ROOT_IO);
    
    //mem
//    float bb[5][2];
    struct flt3 *bb = malloc(10*sizeof(struct flt3));
    
    //open
    file1 = fopen(file1_name,"rb");
    
    fseek(file1, 0, SEEK_END);
    long n = ftell(file1)/sizeof(struct flt3);
    fseek(file1, 0, SEEK_SET);
    printf("%ld\n", n);

    
    fread(bb, sizeof(struct flt3), n, file1);
    fclose(file1);
    

    for (int i = 0; i < n; i++)
    {
        printf("%f %f %f\n", bb[i].x, bb[i].y, bb[i].z);
    }

    free(bb);
    
    
    
    return;
}




#endif /* io_h */
