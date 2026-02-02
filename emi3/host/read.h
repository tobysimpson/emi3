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




#endif // !read_h
