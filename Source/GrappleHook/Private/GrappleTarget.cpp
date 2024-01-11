// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleTarget.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGrappleTarget::AGrappleTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(Root);
	TargetMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	TargetMesh->SetCollisionObjectType(ECC_WorldStatic);
	TargetMesh->CanCharacterStepUp(false);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	CollisionSphere->SetupAttachment(Root);
	CollisionSphere->SetSphereRadius(40.f);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	CharacterLandingLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterLandingLocation"));
	CharacterLandingLocation->SetupAttachment(TargetMesh);
	CharacterLandingLocation->SetRelativeLocation(FVector(300.f, 0.f, 600.f));

	TargetIconBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("TargetIcon"));
	TargetIconBillboard->SetupAttachment(Root);

	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidget"));
	TargetWidget->SetupAttachment(TargetIconBillboard);
	TargetWidget->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetWidget->SetVisibility(false);

	//ShowTargetWidget(false);
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

void AGrappleTarget::ShowTargetWidget(bool bShow)
{
	TargetWidget->SetVisibility(bShow);
}