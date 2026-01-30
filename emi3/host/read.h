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
long file_len(FILE* file1)
{
    fseek(file1, 0, SEEK_END);
    long n = ftell(file1);
    fseek(file1, 0, SEEK_SET);
    
    return n;
}


void read_dbl3(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s.raw", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = file_len(file1)/sizeof(struct dbl3);
    printf("%s %ld\n", dsc, n);

    struct dbl3 *bb = malloc(n*sizeof(struct dbl3));
    
    fread(bb, sizeof(struct dbl3), n, file1);
    fclose(file1);
    

    for (int i = 0; i < 10; i++)
    {
        printf("%e %e %e\n", bb[i].x, bb[i].y, bb[i].z);
    }

    free(bb);

    return;
}


void read_lng4(char* dsc)
{
    FILE* file1;
    char file1_name[250];
    
    sprintf(file1_name, "%s/%s.raw", ROOT_READ, dsc);
    
    file1 = fopen(file1_name,"rb");
    
    long n = file_len(file1)/sizeof(struct lng4);
    printf("%s %ld\n", dsc, n);
    

    struct lng4 *bb = malloc(n*sizeof(struct lng4));
    
    fread(bb, sizeof(struct lng4), n, file1);
    fclose(file1);
    

    for (int i = 0; i < 10; i++)
    {
        printf("%6ld %6ld %6ld %6ld\n", bb[i].x, bb[i].y, bb[i].z, bb[i].w);
    }

    free(bb);

    return;
}




#endif // !read_h
