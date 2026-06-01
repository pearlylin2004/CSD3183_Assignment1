#include <pch.h>
#include "D_Succeeder.h"

void D_Succeeder::on_update(float dt){
    BehaviorNode* child = children.front();
    child->tick(dt);

    if (child->succeeded() || child->failed())    {
        on_success();
    }
}
