//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef read_h
#define read_h

#define ROOT_READ   "/Users/toby/Downloads"



//file length bytes
long flen(FILE* file1)
{
    fseek(file1, 0, SEEK_END);
    long n = ftell(file1);
    fseek(file1, 0, SEEK_SET);
    
    return n;
}


void read_flt3(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = flen(file1)/sizeof(struct flt3);
    printf("%s %ld\n", dsc, n);

    struct flt3 *bb = malloc(n*sizeof(struct flt3));
    
    fread(bb, sizeof(struct flt3), n, file1);
    fclose(file1);
    
    for (int i = 0; i < 10; i++)
    {
        printf("%e %e %e\n", bb[i].x, bb[i].y, bb[i].z);
    }

    free(bb);

    return;
}


void read_int1(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = flen(file1)/sizeof(int);
    printf("%s %ld\n", dsc, n);

    int *bb = malloc(n*sizeof(int));
    
    fread(bb, sizeof(long), n, file1);
    fclose(file1);

    for (int i = 0; i < 10; i++)
    {
        printf("%6d\n", bb[i]);
    }

    free(bb);

    return;
}


void read_int3(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = flen(file1)/sizeof(struct int3);
    printf("%s %ld\n", dsc, n);
    
    struct int3 *bb = malloc(n*sizeof(struct int3));
    
    fread(bb, sizeof(struct int3), n, file1);
    fclose(file1);
    
    for (int i = 0; i < 10; i++)
    {
        printf("%6d %6d %6d\n", bb[i].x, bb[i].y, bb[i].z);
    }

    free(bb);

    return;
}


void read_int4(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = flen(file1)/sizeof(struct int4);
    printf("%s %ld\n", dsc, n);
    
    struct int4 *bb = malloc(n*sizeof(struct int4));
    
    fread(bb, sizeof(struct int4), n, file1);
    fclose(file1);
    
    for (int i = 0; i < 10; i++)
    {
        printf("%6d %6d %6d %6d\n", bb[i].x, bb[i].y, bb[i].z, bb[i].w);
    }

    free(bb);

    return;
}




#endif // !read_h
