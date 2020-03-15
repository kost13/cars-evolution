#include "World.h"

physics::World::World() : vec_{3, 4} {}

float physics::World::vecNorm() const { return vec_.Length(); }
