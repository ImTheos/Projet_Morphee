// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/Ball/BallEffect.h"

bool UBallEffect::operator==(const UBallEffect& otherEffect) const
{
	return effectName == otherEffect.effectName;
}
