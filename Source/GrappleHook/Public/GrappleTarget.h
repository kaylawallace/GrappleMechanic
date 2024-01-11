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

	const FVector GetCharacterLandingLocation();

	void ShowTargetWidget(bool bShow);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> TargetMesh;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USceneComponent> CharacterLandingLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UBillboardComponent> TargetIconBillboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<class UWidgetComponent> TargetWidget;

//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MakeEditWidget))
//		FVector CharacterOffset = FVector(0.f, 0.f, 100.f);
};
