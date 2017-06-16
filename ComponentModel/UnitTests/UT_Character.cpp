#pragma once

#include "gtest/gtest.h"

#include "scl/MathLib.h"
#include "GameComponents/Character/CharacterMoveController.h"

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

	GameTick::Init();
	GameTick::UpdateTick(0);

	auto& rb = obj->Transform();
	rb.SetAngVelocity(1);
	EXPECT_EQ(rb.Position(), Vector3f(0, 0, 0));
	EXPECT_FLOAT_EQ(rb.Yaw(), 0);

	GameTick::UpdateTick(100);
	EXPECT_EQ(rb.Position(), Vector3f(0, 0, 0));
	EXPECT_FLOAT_EQ(rb.Yaw(), 0);

	Vector3f targetPosition = Vector3f(100, 0, 0);
	targetPosition = targetPosition.Rotate2d(10);
	target->Transform().SetPosition(targetPosition);
	GameTick::UpdateTick(200);
	EXPECT_FLOAT_EQ(rb.Yaw(), 10);

	targetPosition = targetPosition.Rotate2d(10);
	target->Transform().SetPosition(targetPosition);
	GameTick::UpdateTick(300);
	EXPECT_FLOAT_EQ(rb.Yaw(), 20);
}

