// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleTarget.h"
#include "Components/SphereComponent.h"

// Sets default values
AGrappleTarget::AGrappleTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);

	/*SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);*/

	TargetMesh->SetCollisionObjectType(ECC_WorldStatic);
}

// Called when the game starts or when spawned
void AGrappleTarget::BeginPlay()
{
	Super::BeginPlay();
	
}