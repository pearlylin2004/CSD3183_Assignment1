#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_CheckRabbitNearby : public BaseNode<L_CheckRabbitNearby>{
protected:
    virtual void on_update(float dt) override;
private:
    static constexpr float DETECTION_RANGE = 40.0f;
};
