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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	TObjectPtr<class AGrappleHookCharacter> OwningCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core")
	EGrappleState GrappleState;

	UPROPERTY(EditDefaultsOnly, Category="GrappleComponentClasses")
	TSubclassOf<class AGrappleEndPoint> GrappleEndPointClass;

	UPROPERTY(EditDefaultsOnly, Category = "GrappleComponentsClasses")
	TSubclassOf<class AGrappleCable> GrappleCableClass;

	TObjectPtr<AGrappleTarget> CurrentTarget;

	TObjectPtr<class AGrappleEndPoint> EndPoint; 

	TObjectPtr<class AGrappleCable> GrappleCable;

	UPROPERTY(EditAnywhere, Category = "Core")
		float MaxGrappleRange = 1500.f;

	// Distance between grapple end point and target in which the player will start grapple movement/action
	UPROPERTY(EditAnywhere, Category = "Core")
	float StartGrappleDistance = 500.f;

	// Distance between the owning character and grapple target at which the player will receive a boost before landing at grapple target
	UPROPERTY(EditAnywhere, Category = "Core")
	float EndGrappleDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Core")
	float CharacterGrappleSpeed = 1500.f;

	FVector InitLaunchDirection = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Core")
	float MinFinalBoost = 400.f;

	UPROPERTY(EditAnywhere, Category = "Core")
	float MaxFinalBoost = 1000.f;

private:
	float Steepness = 0.f;

	float FinalBoost = 0.f;
};
