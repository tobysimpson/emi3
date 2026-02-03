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
    sprintf(file1_name, "%s/msh.%02d.xmf", ROOT_WRITE, idx);
    
    //open
    file1 = fopen(file1_name,"w");
    
    fprintf(file1,"<?xml version=\"1.0\"?>\n");
    fprintf(file1,"<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"3.0\">\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Grid Name=\"ele\">\n");
    fprintf(file1,"      <Geometry Type=\"XYZ\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Float\" Precision=\"4\" Endian=\"Little\" Dimensions=\"24335 3\">vtx_xyz.raw</DataItem>\n");
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Topology TopologyType=\"Tetrahedron\" NumberOfElements=\"140004\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"140004 4\">ele_vtx.raw</DataItem>\n");
    fprintf(file1,"      </Topology>\n");
    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"ele_gg\" AttributeType=\"Scalar\" Center=\"Cell\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"140004\">ele_tag.raw</DataItem>\n");
    fprintf(file1,"      </Attribute>\n");
    fprintf(file1,"    </Grid>\n");
    fprintf(file1,"    <Grid Name=\"fac\">\n");
//    fprintf(file1,"      <xi:include xpointer=\"xpointer(/Xdmf/Domain/Grid/Geometry)\"/>\n"); //works
    fprintf(file1,"      <Geometry Type=\"XYZ\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Float\" Precision=\"4\" Endian=\"Little\" Dimensions=\"24335 3\">vtx_xyz.raw</DataItem>\n");
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Topology TopologyType=\"Triangle\" NumberOfElements=\"282582\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"282582 3\">fac_vtx.raw</DataItem>\n");
    fprintf(file1,"      </Topology>\n");
    fprintf(file1,"      <Attribute  ItemType=\"Uniform\" Name=\"fac_gg\" AttributeType=\"Scalar\" Center=\"Cell\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"282582\">fac_tag.raw</DataItem>\n");
    fprintf(file1,"      </Attribute>\n");
    fprintf(file1,"    </Grid>\n");
    fprintf(file1,"  </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
        
    //clean up
    fclose(file1);
}

#endif /* write_h */

