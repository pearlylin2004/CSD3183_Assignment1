#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_Wander : public BaseNode<L_Wander>{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    Vec3 wanderTarget;
    static constexpr float WANDER_RADIUS = 40.0f;
};
