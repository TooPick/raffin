#ifndef Camera_HPP
#define Camera_HPP

#include "struct.h"

class Camera
{
  public:
    point3 eye;
    point3 at;
    point3 up;
    double angle;
    double offset;
    double distance;

    Camera(point3, point3, point3);
    void set();
    void update(point3, point3, point3);
};

#endif
