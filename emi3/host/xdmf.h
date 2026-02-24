//
//  xdmf.h
//  emi3
//
//  Created by Toby Simpson on 18.02.2026.
//

#ifndef xdmf_h
#define xdmf_h


//write xdmf
void write_xmf(struct vxl_obj *vxl, int idx)
{
    FILE* file1;
    char file1_name[250];
    
    //file name
    sprintf(file1_name, "%s/vxl.%03d.xmf", ROOT_WRITE, idx);
    
    //open
    file1 = fopen(file1_name,"w");
    
    fprintf(file1,"<?xml version=\"1.0\"?>\n");
    fprintf(file1,"<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"3.0\">\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Grid Name=\"vxl\" GridType=\"Uniform\">\n");
    fprintf(file1,"      <Topology name=\"topo\" TopologyType=\"3DCoRectMesh\" Dimensions=\"%d %d %d\"></Topology>\n", vxl->vtx.dim.z, vxl->vtx.dim.y, vxl->vtx.dim.x);
    fprintf(file1,"      <Geometry name=\"geo\" Type=\"ORIGIN_DXDYDZ\">\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">0 0 0</DataItem>\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">%e %e %e</DataItem>\n", vxl->dx, vxl->dx, vxl->dx);
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"gg\" Center=\"Cell\" AttributeType=\"Scalar\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" Dimensions=\"%d %d %d\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">gg.000.dat</DataItem>\n", vxl->ele.dim.z, vxl->ele.dim.y, vxl->ele.dim.x);
    fprintf(file1,"      </Attribute>\n");
    
//    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"uu\" Center=\"Cell\" AttributeType=\"Scalar\">\n");
//    fprintf(file1,"        <DataItem Format=\"Binary\" Dimensions=\"%d %d %d\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">uu.%03d.dat</DataItem>\n", vxl->ele.dim.z, vxl->ele.dim.y, vxl->ele.dim.x, idx);
//    fprintf(file1,"      </Attribute>\n");
    
    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"uu\" Center=\"Cell\" AttributeType=\"Matrix\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" Dimensions=\"%d %d %d 2\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">uu.%03d.dat</DataItem>\n", vxl->ele.dim.z, vxl->ele.dim.y, vxl->ele.dim.x, idx);
    fprintf(file1,"      </Attribute>\n");

    fprintf(file1,"    </Grid>\n");
    fprintf(file1,"  </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
    
    //clean up
    fclose(file1);
}

#endif /* xdmf_h */
