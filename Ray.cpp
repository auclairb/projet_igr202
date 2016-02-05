#include "Ray.h"

static float epsilon = 0.000001f;

Ray::Ray(Vec3f _origin,Vec3f _direction){
    this->origin = _origin;
    this->direction = _direction;
}

bool Ray::intersects(Vec3f p0, Vec3f p1, Vec3f p2){
    Vec3f o = this->origin;
    Vec3f w = this->direction;
    Vec3f e0 = p1 - p0;
    Vec3f e1 = p2 - p0;
    Vec3f n = cross(e0,e1);
    n.normalize();
    Vec3f q = cross(w,e1);
    Vec3f a = dot(e0,q);

    if (dot(n,w)>=0.0f || a.length() < epsilon)
        return false;
    Vec3f s = (o - p0)/a;
    Vec3f r = cross(s,e0);
    float b0 = dot(s,q);
    float b1 = dot(r,w);
    float b2 = 1.0f - b0 - b1;

    if (b0 < 0.0f || b1 < 0.0f || b2 < 0.0f)
        return false;
    float t = dot(e1,r);
    
    if (t>=0.0f)
        return true;
    return false;   
}

bool Ray::intersects(Mesh& mesh){
    for (unsigned int i = 0; i < mesh.T.size(); i++) {
        const Vertex & v0 = mesh.V[mesh.T[i].v[0]];
        const Vertex & v1 = mesh.V[mesh.T[i].v[1]];
        const Vertex & v2 = mesh.V[mesh.T[i].v[2]];
        if (this->intersects(v0.p,v1.p,v2.p))
            return true;
    }
    return false;
}

