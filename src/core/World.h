#ifndef WORLD_H
#define WORLD_H

#include "box2d/b2_body.h"

namespace physics {
class World
{
public:
    explicit World();
    int posY();

private:
 b2Vec2 vec_;

};

}


#endif // WORLD_H
