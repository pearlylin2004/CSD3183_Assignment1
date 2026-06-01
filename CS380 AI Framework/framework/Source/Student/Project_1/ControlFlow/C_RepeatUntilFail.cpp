#include <pch.h>
#include "C_RepeatUntilFail.h"

void C_RepeatUntilFail::on_update(float dt){
    BehaviorNode* child = children.front();
    child->tick(dt);

    if (child->failed()){
        on_success();
    }
    else if (child->succeeded()){
        child->set_status(NodeStatus::READY);
    }

    //will stay running if its still running
}
