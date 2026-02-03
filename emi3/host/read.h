//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef read_h
#define read_h

#define ROOT_READ   "/Users/toby/Downloads"



//file length in bytes
long flen(FILE* file1)
{
    fseek(file1, 0, SEEK_END);
    long n = ftell(file1);
    fseek(file1, 0, SEEK_SET);
    return n;
}




long read_flt3(char* file_name, struct flt3 **buf)
{
    FILE* file_ptr;
    char file_path[250];
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long n = flen(file_ptr)/sizeof(struct flt3);
    *buf = malloc(n*sizeof(struct flt3));
    
    //read
    fread(*buf, sizeof(struct flt3), n, file_ptr);
    fclose(file_ptr);
    
    return n;
}


long read_int3(char* file_name, struct int3 **buf)
{
    FILE* file_ptr;
    char file_path[250];
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long n = flen(file_ptr)/sizeof(struct int3);
    *buf = malloc(n*sizeof(struct int3));
    
    //read
    fread(*buf, sizeof(struct int3), n, file_ptr);
    fclose(file_ptr);
    
    return n;
}


long read_int4(char* file_name, struct int4 **buf)
{
    FILE* file_ptr;
    char file_path[250];
    
    //open
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    
    //allocate
    long n = flen(file_ptr)/sizeof(struct int4);
    *buf = malloc(n*sizeof(struct int4));
    
    //read
    fread(*buf, sizeof(struct int4), n, file_ptr);
    fclose(file_ptr);
    
    return n;
}


#endif // !read_h
