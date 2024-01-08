// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EGrappleState : uint8
{
	Retracted,
	Firing,
	NearingTarget,
	OnTarget
};

/**
 * 
 */
class GRAPPLEHOOK_API GlobalData_Grapple
{
public:
	GlobalData_Grapple();
	~GlobalData_Grapple();
};
