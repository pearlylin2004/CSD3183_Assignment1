#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_CheckFairyNearby : public BaseNode<L_CheckFairyNearby>{
protected:
    virtual void on_update(float dt) override;
private:
    static constexpr float DETECTION_RANGE = 30.0f;
};
