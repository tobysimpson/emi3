//
//  main.c
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#include <stdlib.h>
#include <stdio.h>

#include "vec.h"
#include "read.h"
#include "write.h"

//working with mesh
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    printf("%s\n", ROOT_READ);
    printf("%s\n", ROOT_WRITE);
    
    read_flt3("vtx_xx.raw");
    read_int4("ele_aa.raw");
    read_int1("ele_gg.raw");
    read_int3("fac_aa.raw");
    read_int1("fac_gg.raw");
    
    write_xmf(0);
    
    printf("done\n");
    return EXIT_SUCCESS;
}
