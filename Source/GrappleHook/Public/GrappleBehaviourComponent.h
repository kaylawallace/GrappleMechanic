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

	void TryGrapple();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AGrappleHookCharacter> OwningCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGrappleState GrappleState;

	UPROPERTY(EditAnywhere)
	float MaxGrappleRange = 1500.f;

	TObjectPtr<AGrappleTarget> CurrentTarget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGrappleEndPoint> GrappleEndPointClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGrappleCable> GrappleCableClass;

	TObjectPtr<class AGrappleEndPoint> EndPoint; 

	TObjectPtr<class AGrappleCable> GrappleCable;

	// Distance between grapple end point and target in which the player will start grapple movement/action
	UPROPERTY(EditAnywhere)
	float StartGrappleDistance = 500.f;

	// Distance between the owning character and grapple target at which the player will receive a boost before landing at grapple target
	UPROPERTY(EditAnywhere)
	float EndGrappleDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float CharacterGrappleSpeed = 1500.f;

	FVector InitLaunchDirection = FVector(0.f);

	UPROPERTY(EditAnywhere)
	float MinFinalBoost = 400.f;

	UPROPERTY(EditAnywhere)
	float MaxFinalBoost = 1000.f;

private:
	float Steepness = 0.f;

	float FinalBoost = 0.f;
};
