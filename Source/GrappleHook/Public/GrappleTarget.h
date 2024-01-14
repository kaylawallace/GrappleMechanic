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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<UStaticMeshComponent> TargetMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USceneComponent> CharacterLandingLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<class USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		TObjectPtr<class UBillboardComponent> TargetIconBillboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		TObjectPtr<class UWidgetComponent> TargetWidget;


// Use of this may be better/more usable in editor than a scene component?
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget), Category = "Offsets")
//		FVector CharacterOffset = FVector(0.f, 0.f, 100.f);
};
