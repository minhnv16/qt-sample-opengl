#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <math.h>

struct Position{
    float x, y, z;
    Position(float _x, float  _y, float  _z){
        x=_x,y=_y,z=_z;
    };
    Position(){};
};
struct Color{
    float r, g, b;
    Color(float _r, float _g, float _b){
        r=_r,g=_g,b=_b;
    };
    Color(){};
};
struct TextureCoord{
    float x, y;
    TextureCoord(float _x, float  _y){
        x=_x,y=_y;
    };
    TextureCoord(){};
};
struct VertexAtt{
    Position pos;
    Color color;
    TextureCoord texCord;
    VertexAtt(Position _pos, Color _color, TextureCoord _texCord)
    {
        pos = _pos;
        color = _color;
        texCord = _texCord;
    };
};

struct Object{
    std::vector<VertexAtt> vecVertex;
    int nIndexInVertices = 0;

};
bool get_area_centroid(const Object &obj,float &a, float &x, float &y) ;
void rotate_point(const float cx, const float cy, float angle, float &x, float &y);
void rotate_my_obj(Object & obj, const float cx, const float cy, float angle) ;



class Common
{
public:
    Common();
};

#endif // COMMON_H
