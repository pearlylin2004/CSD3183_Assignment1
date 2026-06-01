#include <pch.h>
#include "D_RepeatUntilSuccess.h"

void D_RepeatUntilSuccess::on_update(float dt){
    BehaviorNode* child = children.front();
    child->tick(dt);

    if (child->succeeded()){
        on_success();
    }
    else if (child->failed()){
        child->set_status(NodeStatus::READY);
    }
}
