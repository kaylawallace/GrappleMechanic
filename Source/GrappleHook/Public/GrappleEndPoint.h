// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GrappleEndPoint.generated.h"

UCLASS()
class GRAPPLEHOOK_API AGrappleEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleEndPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetDirection();

	void SetEndLocation(FVector InLocation);

	void SetGrappleTarget(class AGrappleTarget* InTarget);

	const float GetDistanceToTarget();

private: 
	virtual void OnConstruction(const FTransform& transform) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AGrappleTarget> GrappleTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector EndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Direction;
};
