// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleHook/GrappleHookCharacter.h"
#include "GrappleTarget.generated.h"

UCLASS(Blueprintable)
class GRAPPLEHOOK_API AGrappleTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> TargetMesh;

	/*UPROPERTY(EditAnywhere)
		TObjectPtr<class USphereComponent> SphereCollision;*/
};
