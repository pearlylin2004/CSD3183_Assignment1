#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_CheckNearbyAgent : public BaseNode<L_CheckNearbyAgent>{
protected:
    virtual void on_update(float dt) override;
private:
    static constexpr float PROXIMITY_THRESHOLD = 30.0f;
};
