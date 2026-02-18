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
    sprintf(file1_name, "%s/vxl.%02d.xmf", ROOT_WRITE, idx);
    
    //open
    file1 = fopen(file1_name,"w");
    
    fprintf(file1,"<?xml version=\"1.0\"?>\n");
    fprintf(file1,"<Xdmf xmlns:xi=\"http://www.w3.org/2001/XInclude\" Version=\"3.0\">\n");
    fprintf(file1,"  <Domain>\n");
    fprintf(file1,"    <Grid Name=\"vxl\" GridType=\"Uniform\">\n");
    fprintf(file1,"      <Topology name=\"topo\" TopologyType=\"3DCoRectMesh\" Dimensions=\"%d %d %d\"></Topology>\n", vxl->nv.z, vxl->nv.y, vxl->nv.x);
    fprintf(file1,"      <Geometry name=\"geo\" Type=\"ORIGIN_DXDYDZ\">\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">%e %e %e</DataItem>\n", vxl->x0.x, vxl->x0.y, vxl->x0.z);
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">%e %e %e</DataItem>\n", vxl->dx, vxl->dx, vxl->dx);
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"vxl_tag\" Center=\"Cell\" AttributeType=\"Scalar\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" Dimensions=\"%d %d %d\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">vxl_tag.dat</DataItem>\n", vxl->ne.z, vxl->ne.y, vxl->ne.x);
    fprintf(file1,"      </Attribute>\n");
    
//    fprintf(file1,"      <Attribute Name=\"gg\" Center=\"Cell\" AttributeType=\"Matrix\">\n");
//    fprintf(file1,"        <DataItem Format=\"Binary\" Dimensions=\"%llu %llu %llu 4\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", vxl->ne.z, vxl->ne.y, vxl->ne.x);
//    fprintf(file1,"             /Users/toby/Downloads/raw/gg.%02u%02u%02u.%03d.raw\n", vxl->le.x, vxl->le.y, vxl->le.z, idx);
//    fprintf(file1,"           </DataItem>\n");
//    fprintf(file1,"         </Attribute>\n");
//    fprintf(file1,"       <Attribute Name=\"uu\" Center=\"Node\" AttributeType=\"Vector\">\n");
//    fprintf(file1,"         <DataItem Format=\"Binary\" Dimensions=\"%llu %llu %llu 3\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", vxl->nv.z, vxl->nv.y, vxl->nv.x);
//    fprintf(file1,"             /Users/toby/Downloads/raw/uu.%02u%02u%02u.%03d.raw\n", vxl->le.x, vxl->le.y, vxl->le.z, idx);
//    fprintf(file1,"         </DataItem>\n");
//    fprintf(file1,"       </Attribute>\n");

    fprintf(file1,"    </Grid>\n");
    fprintf(file1,"  </Domain>\n");
    fprintf(file1,"</Xdmf>\n");
    
    //clean up
    fclose(file1);
}

#endif /* xdmf_h */
