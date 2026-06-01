#include <pch.h>
#include "L_PlayMusic.h"

void L_PlayMusic::on_enter(){
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    BehaviorNode::on_leaf_enter();
    on_success();
}
