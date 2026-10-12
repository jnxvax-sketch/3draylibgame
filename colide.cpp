#include "colide.h"
#include <raymath.h>
namespace Collide {
    bool CheckSphereCollision(Vector3 pos1, float radius1, Vector3 pos2, float radius2) {
        float distance = Vector3Distance(pos1, pos2);
        return distance <= (radius1 + radius2);
    }
}