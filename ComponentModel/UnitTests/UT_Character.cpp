#pragma once

#include "gtest/gtest.h"

#include "scl/MathLib.h"
#include "GameEngine/GameComponents/Character/CharacterMoveController.h"

using namespace scl;
using namespace GameEngine;

TEST(CharacterMoveController, MoveWithTarget)
{
	auto obj = New<GameEngine::GameObject>();
	obj->SetComponent<CharacterMoveController>();

	auto target = New<GameEngine::GameObject>();
	target->SetComponent<CharacterMoveController>();

	auto movController = obj->GetComponent<CharacterMoveController>();
	EXPECT_EQ(movController != nullptr, true);

	movController->SetTarget(target);
	
	int64 tick = 0;
	auto& rb = obj->RigidBody();
	rb.SetAngVelocity(1, tick);
	EXPECT_EQ(rb.Position(tick), Vector3f(0, 0, 0));
	EXPECT_FLOAT_EQ(rb.Yaw(tick), 0);

	tick += 100;
	EXPECT_EQ(rb.Position(tick), Vector3f(0, 0, 0));
	EXPECT_FLOAT_EQ(rb.Yaw(tick), 0);

	Vector3f targetPosition = Vector3f(100, 0, 0);

	tick += 100;
	targetPosition = targetPosition.Rotate2d(10);
	target->RigidBody().SetPosition(targetPosition, tick);
	EXPECT_FLOAT_EQ(rb.Yaw(tick), 10);

	tick += 100;
	targetPosition = targetPosition.Rotate2d(10);
	target->RigidBody().SetPosition(targetPosition, tick);
	EXPECT_FLOAT_EQ(rb.Yaw(tick), 20);
}

