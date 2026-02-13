//
//  io.h
//  emi3
//
//  Created by Toby Simpson on 05.02.2026.
//

#ifndef io_h
#define io_h


#define ROOT_READ   "/Users/toby/Downloads"
#define ROOT_WRITE  "/Users/toby/Downloads"

/*
 =============================
 xdmf
 =============================
 */

//write xdmf
void write_xmf(struct tet_obj *tet, struct vxl_obj *vxl, int idx)
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
    fprintf(file1,"    <Grid Name=\"tet\">\n");
    fprintf(file1,"      <Geometry Type=\"XYZ\">\n");
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Float\" Precision=\"4\" Endian=\"Little\" Dimensions=\"%ld 3\">vtx_xyz.dat</DataItem>\n", tet->nv);
    fprintf(file1,"      </Geometry>\n");
    fprintf(file1,"      <Topology TopologyType=\"Tetrahedron\" NumberOfElements=\"%ld\">\n", tet->ne);
    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"%ld 4\">tet_vtx.dat</DataItem>\n", tet->ne);
    fprintf(file1,"      </Topology>\n");
//    fprintf(file1,"      <Attribute ItemType=\"Uniform\" Name=\"ele_tag\" AttributeType=\"Scalar\" Center=\"Cell\">\n");
//    fprintf(file1,"        <DataItem Format=\"Binary\" DataType=\"Int\" Precision=\"4\" Endian=\"Little\" Dimensions=\"%ld\">ele_tag.raw</DataItem>\n", tet->ne);
//    fprintf(file1,"      </Attribute>\n");
    fprintf(file1,"    </Grid>\n");

    fprintf(file1,"    <Grid Name=\"vxl\" GridType=\"Uniform\">\n");
    fprintf(file1,"      <Topology name=\"topo\" TopologyType=\"3DCoRectMesh\" Dimensions=\"%d %d %d\"></Topology>\n", vxl->nv.z, vxl->nv.y, vxl->nv.x);
    fprintf(file1,"      <Geometry name=\"geo\" Type=\"ORIGIN_DXDYDZ\">\n");
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">%f %f %f</DataItem>\n", 0e0f, 0e0f, 0e0f);
    fprintf(file1,"        <DataItem Format=\"XML\" Dimensions=\"3\">%f %f %f</DataItem>\n", vxl->dx, vxl->dx, vxl->dx);
    fprintf(file1,"      </Geometry>\n");
    
    fprintf(file1,"       <Attribute Name=\"vxl_dat\" Center=\"Cell\" AttributeType=\"Matrix\">\n");
    fprintf(file1,"         <DataItem Format=\"Binary\" Dimensions=\"%d %d %d 4\" Endian=\"Little\" Precision=\"4\" NumberType=\"Float\">\n", vxl->ne.z, vxl->ne.y, vxl->ne.x);
    fprintf(file1,"             /Users/toby/Downloads/vxl_dat.dat\n");
    fprintf(file1,"         </DataItem>\n");
    fprintf(file1,"       </Attribute>\n");
    
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

/*
 =============================
 util
 =============================
 */

//file length in bytes
size_t file_size(char* file_name)
{
    FILE*   file_ptr;
    char    file_path[250];
    
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    fseek(file_ptr, 0, SEEK_END);
    size_t n = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    fclose(file_ptr);
    
    return n;
}

/*
 =============================
 read
 =============================
 */



//file to cl_mem
void read_flt4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    //name
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    
    //map
    FILE*       file_ptr = fopen(file_path,"rb");
    cl_float4*  host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*sizeof(cl_float4), 0, NULL, NULL, &ocl->err);
    
    //read
    for (int i=0; i<n; i++)
    {
        fread(&host_ptr[i], sizeof(cl_float), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}

//file to cl_mem
void read_int4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    FILE*       file_ptr;
    char        file_path[250];
    cl_int4*    host_ptr;
    
    //buffer
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_READ, 0, n*sizeof(cl_int4), 0, NULL, NULL, &ocl->err);
    
    for (int i=0; i<n; i++)
    {
        fread(&host_ptr[i], sizeof(cl_int), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}

/*
 =============================
 write
 =============================
 */

//cl_mem to file
void write_int4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    
    //name
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    
    FILE*    file_ptr = fopen(file_path,"wb");
    cl_int4* host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_WRITE, 0, n*sizeof(cl_int4), 0, NULL, NULL, &ocl->err);
    
    for (int i=0; i<n; i++)
    {
        fwrite(&host_ptr[i], sizeof(cl_int), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}

//cl_mem to file
void write_flt4(struct ocl_obj *ocl, char *file_name, cl_mem *buf, size_t n, int w)
{
    
    //name
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    
    FILE*    file_ptr = fopen(file_path,"wb");
    cl_float4* host_ptr = clEnqueueMapBuffer(ocl->command_queue, *buf, CL_TRUE, CL_MAP_WRITE, 0, n*sizeof(cl_float4), 0, NULL, NULL, &ocl->err);
    
    for (int i=0; i<n; i++)
    {
        fwrite(&host_ptr[i], sizeof(cl_float), w, file_ptr);
    }
    
    clEnqueueUnmapMemObject(ocl->command_queue, *buf, host_ptr, 0, NULL, NULL);
    
    //clean up
    fclose(file_ptr);
    
    return;
}



#endif /* io_h */
