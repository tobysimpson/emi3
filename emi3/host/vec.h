//
//  vec.h
//  emi3
//
//  Created by Toby Simpson on 16.02.2026.
//

#ifndef vec_h
#define vec_h

struct int3
{
    int x;
    int y;
    int z;
};

struct int4
{
    int x;
    int y;
    int z;
    int w;
};

struct flt3
{
    float x;
    float y;
    float z;
};

struct flt4
{
    float x;
    float y;
    float z;
    float w;
};



float dot3(struct flt3 a, struct flt3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

struct flt3 cross(struct flt3 a, struct flt3 b)
{
    return (struct flt3){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

float det3(struct flt3 a, struct flt3 b, struct flt3 c)
{
    return dot3(a, cross(b, c));
}


#endif /* vec_h */
