//
//  io_write.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef write_h
#define write_h


#define ROOT_WRITE   "/Users/toby/Downloads"


//write xdmf
void write_xmf(int idx)
{
    FILE* file1;
    char file1_name[250];
    
    //file name
    sprintf(file1_name, "%s/msh.%02d.xdmf", ROOT_WRITE, idx);
    
    //open
    file1 = fopen(file1_name,"w");
    
    fprintf(file1,"<?xml version=\"1.0\"?>\n");
    fprintf(file1,"<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"3.0\">\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Grid Name=\"tets\">\n");
    fprintf(file1,"      <Geometry Type=\"XYZ\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Float\" Precision=\"4\" Endian=\"Little\" Dimensions=\"24335 3\">geom.raw</DataItem>\n");
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Topology Type=\"Tetrahedron\" NumberOfElements=\"140004\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"140004 4\">topo.raw</DataItem>\n");
    fprintf(file1,"      </Topology>\n");
    fprintf(file1,"      <Attribute Name=\"tags\" AttributeType=\"Scalar\" Center=\"Cell\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"140004\">tags.raw</DataItem>\n");
    fprintf(file1,"      </Attribute>\n");
    fprintf(file1,"    </Grid>\n");
    fprintf(file1,"  </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
        
    //clean up
    fclose(file1);
}

#endif /* write_h */
