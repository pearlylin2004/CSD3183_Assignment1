#include <pch.h>
#include "L_MoveToCenter.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCenter::on_enter(){
    float halfMap = terrain->mapSizeInWorld * 0.5f;
    centerPoint = Vec3(halfMap, 0.0f, halfMap);
    BehaviorNode::on_leaf_enter();
}

void L_MoveToCenter::on_update(float dt){
    const auto result = agent->move_toward_point(centerPoint, dt);
    if (result) on_success();
    display_leaf_text();
}
