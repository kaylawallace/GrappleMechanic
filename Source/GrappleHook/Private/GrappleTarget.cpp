// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleTarget.h"
#include "Components/SphereComponent.h"

// Sets default values
AGrappleTarget::AGrappleTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//SetRootComponent(Root);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);
	TargetMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	TargetMesh->SetCollisionObjectType(ECC_WorldStatic);

	CharacterLandingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterLandingLocation"));
	CharacterLandingLocation->SetupAttachment(TargetMesh);
	CharacterLandingLocation->SetRelativeLocation(FVector(300.f, 0.f, 600.f));
}

// Called when the game starts or when spawned
void AGrappleTarget::BeginPlay()
{
	Super::BeginPlay();
}

const FVector AGrappleTarget::GetCharacterLandingLocation()
{
	return CharacterLandingLocation->GetComponentLocation();
}