//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef io_h
#define io_h

#define ROOT_IO   "/Users/toby/Downloads"

void read_raw(void);
//fwrite(ptr1, sizeof(float), msh->nv_tot, file1);

//write raw
void read_raw(void)
{
    FILE* file1;
    char file1_name[250];
    
    
    sprintf(file1_name, "%s/bb.raw", ROOT_IO);
    
    //out
    float bb[10];
    
    file1 = fopen(file1_name,"rb");
    
    fread(bb, sizeof(float), 10, file1);
    
    
    for (int i = 0; i < 10; i++)
    {
        printf("%f\n", bb[i]);
    }
    
    
    //clean up
    fclose(file1);
    
    return;
}




#endif /* io_h */
