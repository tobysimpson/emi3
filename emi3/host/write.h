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
    fprintf(file1,"<Xdmf version=\"3.0\">\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Topology name=\"topo\" TopologyType=\"Tetrahedron\" NumberOfElements=\"140004\">\n");
    fprintf(file1,"      <DataItem Format=\"Binary\" NumberType=\"Int\" Endian=\"Little\" Precision=\"8\" Dimensions=\"140004 4\">topo.raw</DataItem>\n");
    fprintf(file1,"    </Topology>\n");
    fprintf(file1,"    <Geometry name=\"geom\" GeometryType=\"XYZ\">\n");
    fprintf(file1,"      <DataItem Format=\"Binary\" NumberType=\"Float\" Endian=\"Little\" Precision=\"8\" Dimensions=\"24335 3\">geom.raw</DataItem>\n");
    fprintf(file1,"    </Geometry>\n");
    
    fprintf(file1,"      <Grid Name=\"tets\" GridType=\"Uniform\">\n");
    fprintf(file1,"        <Topology Reference=\"/Xdmf/Domain/Topology[1]\"/>\n");
    fprintf(file1,"        <Geometry Reference=\"/Xdmf/Domain/Geometry[1]\"/>\n");
    fprintf(file1,"      </Grid>\n");
    
    //    fprintf(file1,"         <Attribute Name=\"uu\" Center=\"Node\" AttributeType=\"Scalar\">\n");
    //    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
    //    fprintf(file1,"             /Users/toby/Downloads/raw/uu.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
    //    fprintf(file1,"           </DataItem>\n");
    //    fprintf(file1,"         </Attribute>\n");
    //


//    fprintf(file1,"    </Grid>\n");
    fprintf(file1," </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
    
    //clean up
    fclose(file1);
}


#endif /* write_h */
