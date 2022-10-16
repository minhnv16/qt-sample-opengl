#include "common.h"
#include <math.h>

Common::Common()
{

}


bool get_area_centroid(const Object &obj,float &a, float &x, float &y)
{
    a = 0.0f;

    if (obj.vecVertex.size() < 3) {
        return false;
    }
    std::vector<Position> vec_pos;
    for (int i = 0; i < obj.vecVertex.size(); i++) {
        VertexAtt vi = obj.vecVertex[i];
        vec_pos.push_back(vi.pos);
    }

    vec_pos.push_back(obj.vecVertex[0].pos);

    float sum = 0.0f;
    for (int i = 0; i < vec_pos.size()-1; i++) {
        Position pos_i = vec_pos[i];
        Position pos_j = vec_pos[i+1];
        sum += (pos_i.x * pos_j.y - pos_j.x * pos_i.y);
    }
    a = 0.5 * sum;

    float sum_x = 0.0f, sum_y = 0.0f;
    for (int i = 0; i < vec_pos.size() - 1; i++) {
        Position pos_i = vec_pos[i];
        Position pos_j = vec_pos[i + 1];
        sum_x += ((pos_i.x + pos_j.x) * (pos_i.x * pos_j.y - pos_j.x * pos_i.y));
        sum_y += ((pos_i.y + pos_j.y) * (pos_i.x * pos_j.y - pos_j.x * pos_i.y));
    }
    x = sum_x / (6 * a);
    y = sum_y / (6 * a);
//    x+=0.06;
//    y+=0.06;
    return true;

}

void transition_my_obj(Object &obj, const float cx, const float cy)
{
    for (int i = 0; i < obj.vecVertex.size(); i++) {
        obj.vecVertex[i].pos.x += cx;
        obj.vecVertex[i].pos.y += cy;
    }
}


//void rotate_point(const float cx, const float cy, float angle, float &x, float &y)
//{
//    float s = sin(angle);
//    float c = cos(angle);

//    // translate point back to origin:
//    x -= cx;
//    y -= cy;

//    // rotate point
//    float xnew = x * c - y * s;
//    float ynew = x * s + y * c;

//    // translate point back:
//    x = xnew + cx;
//    y = ynew + cy;

//}

//void rotate_my_obj(Object & obj, const float cx, const float cy, float angle)
//{

//    for (int i = 0; i < obj.vecVertex.size(); i++) {
//        rotate_point(cx, cy, angle, obj.vecVertex[i].pos.x, obj.vecVertex[i].pos.y);

//    }
//}
