#pragma once
#include "BehaviorTrees/BehaviorNode.h"

class L_PlayMusic : public BaseNode<L_PlayMusic>{
protected:
    virtual void on_enter() override;
};
