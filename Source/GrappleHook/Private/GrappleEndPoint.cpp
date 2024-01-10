// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleEndPoint.h"
#include "GrappleTarget.h"

// Sets default values
AGrappleEndPoint::AGrappleEndPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	if (!Mesh)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
		Mesh->SetupAttachment(RootComponent);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetGenerateOverlapEvents(false);
	}
	
	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->InitialSpeed = 5000.f;
		ProjectileMovement->MaxSpeed = 5000.f;
	}
}

// Called when the game starts or when spawned
void AGrappleEndPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrappleEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrappleEndPoint::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	StartLocation = GetActorLocation();

	if (GrappleTarget)
	{
		EndLocation = GrappleTarget->GetActorLocation();
		Direction = (EndLocation - StartLocation).GetSafeNormal();

		SetActorRotation(Direction.Rotation());
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No grapple target found"));
	}
}

FVector AGrappleEndPoint::GetDirection()
{
	return Direction;
}

void AGrappleEndPoint::SetEndLocation(FVector InLocation)
{
	EndLocation = InLocation;
}

void AGrappleEndPoint::SetGrappleTarget(AGrappleTarget* InTarget)
{
	GrappleTarget = InTarget;
}

const float AGrappleEndPoint::GetDistanceToTarget()
{
	if (GrappleTarget)
	{
		return this->GetDistanceTo(GrappleTarget);
	}

	return -1.f;
}