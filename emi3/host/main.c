//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>

#include "io.h"

//working with mesh
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    printf("%s\n", ROOT_IO);
    
    read_raw();
    
    printf("done\n");
    return EXIT_SUCCESS;
}
