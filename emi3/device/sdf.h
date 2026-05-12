//
//  sdf.h
//  emi3
//
//  Created by Toby Simpson on 12.05.2026.
//

#ifndef sdf_h
#define sdf_h


//sphere
float sdf_sph(float3 x, float3 c, float r)
{
    return length(x - c) - r;
}


//cuboid
float sdf_cub(float3 x, float3 c, float3 r)
{
    float3 d = fabs(x - c) - r;
    
    return max(d.x,max(d.y,d.z));
}


//smoothed minimum quadratic
float sdf_smin(float a, float b, float k)
{
    k *= 4.0f;
    float h = fmax(k-fabs(a-b), 0.0f)/k;
    return fmin(a,b) - 0.25f*h*h*k;
}


#endif /* sdf_h */
