//
//  io_write.h
//  emi3
//
//  Created by Toby Simpson on 29.01.2026.
//

#ifndef io_write_h
#define io_write_h


#define ROOT_WRITE   "/Users/toby/Downloads"



//write xdmf
void write_xmf(int idx)
{
    FILE* file1;
    char file1_name[250];

    //file name
    sprintf(file1_name, "%s/%02d.xmf", ROOT_WRITE, idx);
    
    //open
    file1 = fopen(file1_name,"w");
    
    fprintf(file1,"<Xdmf>\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Topology name=\"topo\" TopologyType=\"2DCoRectMesh\" Dimensions=\"%llu %llu\"></Topology>\n", msh->nv.x, msh->nv.y);
    fprintf(file1,"      <Geometry name=\"geo\" Type=\"ORIGIN_DXDY\">\n");
    fprintf(file1,"        <!-- Origin -->\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"2\">%e %e</DataItem>\n", 0.0f, 0.0f);
    fprintf(file1,"        <!-- DxDyDz -->\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"2\">%e %e</DataItem>\n", msh->dx, msh->dx);
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Grid Name=\"T1\" GridType=\"Uniform\">\n");
    fprintf(file1,"        <Topology Reference=\"/Xdmf/Domain/Topology[1]\"/>\n");
    fprintf(file1,"        <Geometry Reference=\"/Xdmf/Domain/Geometry[1]\"/>\n");
    
//    fprintf(file1,"         <Attribute Name=\"uu\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/uu.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    
//    fprintf(file1,"         <Attribute Name=\"bb\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/bb.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    
//    fprintf(file1,"         <Attribute Name=\"rr\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/rr.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    
//    fprintf(file1,"         <Attribute Name=\"vv\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/vv.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    
//    fprintf(file1,"         <Attribute Name=\"ww\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/ww.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    
//    fprintf(file1,"         <Attribute Name=\"gg\" Center=\"Node\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"           <DataItem Format=\"Binary\" Dimensions=\"%llu %llu\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", msh->nv.x, msh->nv.y);
//    fprintf(file1,"             /Users/toby/Downloads/raw/gg.%02d%02d.%02d.raw\n", msh->le.x, msh->le.y, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
    
    
    fprintf(file1,"    </Grid>\n");
    fprintf(file1," </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
    
    //clean up
    fclose(file1);
}


#endif /* io_write_h */
