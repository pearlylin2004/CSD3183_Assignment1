#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_CheckWolfNearby : public BaseNode<L_CheckWolfNearby>{
protected:
    virtual void on_update(float dt) override;
private:
    static constexpr float DETECTION_RANGE = 35.0f;
};
