#include <pch.h>
#include "C_RandomSequencer.h"
#include <algorithm>

C_RandomSequencer::C_RandomSequencer() : currentIndex(0) {}

void C_RandomSequencer::on_enter(){
    currentIndex = 0;
    order.resize(children.size());
    for (size_t i = 0; i < order.size(); ++i) order[i] = i;
    for (size_t i = order.size() - 1; i > 0; --i){
        size_t j = RNG::range(static_cast<size_t>(0), i);
        std::swap(order[i], order[j]);
    }
    BehaviorNode::on_enter();
}

void C_RandomSequencer::on_update(float dt){
    if (currentIndex >= children.size()){
        on_success();
        return;
    }

    BehaviorNode* currentNode = children[order[currentIndex]];
    currentNode->tick(dt);

    if (currentNode->failed()){
        on_failure();
    }
    else if (currentNode->succeeded()){
        ++currentIndex;
        if (currentIndex >= children.size()){
            on_success();
        }
    }
}
