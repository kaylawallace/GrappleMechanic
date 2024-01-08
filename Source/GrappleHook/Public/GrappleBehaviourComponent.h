// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "GrappleHook/GrappleHookCharacter.h"
#include "GlobalData/GlobalData_Grapple.h"
#include "GrappleBehaviourComponent.generated.h"

class AGrappleTarget;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GRAPPLEHOOK_API UGrappleBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleBehaviourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void Tick_GrappleRetracted();

	void Tick_GrappleFiring();

	void Tick_GrappleNearingTarget();

	void Tick_GrappleOnTarget();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentTarget(AGrappleTarget* NewTarget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AGrappleHookCharacter> OwningCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGrappleState GrappleState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	/*const*/ float MaxGrappleRange = 1500.f;

	TObjectPtr<AGrappleTarget> CurrentTarget;
};
