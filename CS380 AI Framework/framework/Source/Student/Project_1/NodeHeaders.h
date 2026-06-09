#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes
#include "ControlFlow/C_RandomSequencer.h"

// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_Succeeder.h"
#include "Decorator/D_RepeatUntilSuccess.h"

// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes
#include "Leaf/L_MoveToNearestAgent.h"
#include "Leaf/L_FleeFromWolf.h"
#include "Leaf/L_CheckWolfNearby.h"
#include "Leaf/L_CheckRabbitNearby.h"
#include "Leaf/L_CheckFairyNearby.h"
#include "Leaf/L_MoveToCenter.h"
#include "Leaf/L_PlayMusic.h"
#include "Leaf/L_Wander.h"
#include "Leaf/L_ChaseRabbit.h"
