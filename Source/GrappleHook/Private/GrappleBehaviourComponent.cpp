// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleBehaviourComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <GrappleTarget.h>
#include "GrappleEndPoint.h"
#include "GrappleCable.h"
#include "CableComponent.h"
#include "Engine.h"

//class AGrappleCable;

// Sets default values for this component's properties
UGrappleBehaviourComponent::UGrappleBehaviourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	// Save reference to owning character as type AGrappleHookCharacter
	OwningCharacter = Cast<AGrappleHookCharacter>(GetOwner());
}

void UGrappleBehaviourComponent::SetCurrentTarget(AGrappleTarget* NewTarget)
{
	if (CurrentTarget)
	{
		if (NewTarget == CurrentTarget)
		{
			return;
		}

		// Hide previous current target widget
		CurrentTarget->ShowTargetWidget(false);
	}
	
	CurrentTarget = NewTarget;

	// Show new current target widget
	CurrentTarget->ShowTargetWidget(true);

}

/*
* Run on grapple input pressed by player, tries to perform grapple behaviour to CurrentTarget
*/
void UGrappleBehaviourComponent::TryGrapple()
{
	if (GrappleState != EGrappleState::Retracted || !CurrentTarget)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Exit state for TryGrapple"));

		return;
	}

	FTransform SpawnTransform = FTransform(FRotator(0.f), OwningCharacter->GetActorLocation(), FVector(1.f));

	// Spawn actual grapple hook (EndPoint)
	EndPoint = Cast<AGrappleEndPoint>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		GetWorld(),
		GrappleEndPointClass, 
		SpawnTransform, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		OwningCharacter
	));

	EndPoint->SetGrappleTarget(CurrentTarget);

	UGameplayStatics::FinishSpawningActor(EndPoint, SpawnTransform);

	// Spawn grapple cable 
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GrappleCable = GetWorld()->SpawnActor<AGrappleCable>(
		GrappleCableClass,
		OwningCharacter->GetActorLocation(),
		OwningCharacter->GetActorRotation(),
		SpawnParams
		);

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;

	GrappleCable->AttachToComponent(OwningCharacter->GetMesh(), AttachmentRules, FName("hand_r"));
	GrappleCable->CableComponent->SetAttachEndTo(EndPoint, FName(EndPoint->GetRootComponent()->GetName()));

	OwningCharacter->SetActorRotation(EndPoint->GetDirection().Rotation());

	GrappleState = EGrappleState::Firing;
}

/*
* Search for best grapple target 
*/
void UGrappleBehaviourComponent::Tick_GrappleRetracted()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeFilter;
	ObjectTypeFilter.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> OutActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwningCharacter);

	// Check for Grapple Targets in range
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		OwningCharacter->GetActorLocation(),
		MaxGrappleRange,
		ObjectTypeFilter,
		AGrappleTarget::StaticClass(),
		ActorsToIgnore,
		OutActors
	);

	// Initialise vars used in Best Target calculation
	float SmallestAngle = 360;
	AActor* BestTarget = nullptr;

	// Iterate through Grapple Targets in range
	for (AActor* OutActor : OutActors)
	{
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		FCollisionResponseParams ResponseParams;

		// Check nothing is blocking sight to current Grapple Target
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			OwningCharacter->GetActorLocation(),
			OutActor->GetActorLocation(),
			ETraceTypeQuery::TraceTypeQuery1, // Visibility Channel
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			Hit,
			true,
			FColor::Red,
			FColor::Green,
			0.f
		);

		// Best Target Calculation
		if (Hit.GetActor())
		{
			if (Hit.GetActor() == OutActor)
			{
				FVector Dir = OutActor->GetActorLocation() - OwningCharacter->GetActorLocation();
				FVector NormalizedDir = Dir.GetSafeNormal();
				FVector CameraForward = OwningCharacter->GetFollowCamera()->GetForwardVector();

				double Dot = NormalizedDir.Dot(CameraForward);
				float CurrentAngle = acos(Dot) * (180 / PI);			

				if (CurrentAngle < SmallestAngle || BestTarget == nullptr)
				{
					SmallestAngle = CurrentAngle;
					BestTarget = OutActor;
				}
			}
		}
	}
	
	if (BestTarget)
	{
		SetCurrentTarget(Cast<AGrappleTarget>(BestTarget));
	}
}

/*
* Performs initial boost to player before they start grappling & checks if grapple is nearing the target 
*/
void UGrappleBehaviourComponent::Tick_GrappleFiring()
{
	if (EndPoint->GetDistanceToTarget() < StartGrappleDistance)
	{
		OwningCharacter->LaunchCharacter(FVector(0.f, 0.f, 300.f), true, true);

		GrappleState = EGrappleState::NearingTarget;
	}
}

/*
* Calculates a final boost amount for the player depending on the steepness of the grapple
*/
void UGrappleBehaviourComponent::Tick_GrappleNearingTarget()
{
	if (OwningCharacter->GetVelocity().Z <= 0.f)
	{
		InitLaunchDirection = (CurrentTarget->GetCharacterLandingLocation() - OwningCharacter->GetActorLocation()).GetSafeNormal();
		GrappleState = EGrappleState::OnTarget;

		Steepness = InitLaunchDirection.Rotation().Pitch;

		if (Steepness <= 0.f)
		{
			FinalBoost = MinFinalBoost;
		}
		else
		{
			float SteepnessNormalised = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 80.f), FVector2D(0.f, 1.f), Steepness);
			FinalBoost = FMath::Lerp(MinFinalBoost, MaxFinalBoost, SteepnessNormalised);
		}
	}
}

/*
* Performs final boost for player and cleans up variables
*/
void UGrappleBehaviourComponent::Tick_GrappleOnTarget()
{
	if (OwningCharacter->GetDistanceTo(CurrentTarget) < EndGrappleDistance)
	{
		FVector LaunchVelocity = InitLaunchDirection * MinFinalBoost; 
		LaunchVelocity.Z = FMath::Max(LaunchVelocity.Z, FinalBoost);

		OwningCharacter->LaunchCharacter(LaunchVelocity, true, (OwningCharacter->GetVelocity().Z > 0.f));

		CurrentTarget = nullptr;
		GrappleCable->Destroy();
		GrappleCable = nullptr;
		EndPoint->Destroy();
		EndPoint = nullptr;

		OwningCharacter->SetActorRotation(FRotator(0.f, OwningCharacter->GetActorRotation().Yaw, 0.f));
		GrappleState = EGrappleState::Retracted;	
	}
	else 
	{
		OwningCharacter->GetMovementComponent()->Velocity = InitLaunchDirection * CharacterGrappleSpeed;	
	}
}


// Called every frame
void UGrappleBehaviourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (GrappleState)
	{
	case EGrappleState::Retracted:
		Tick_GrappleRetracted();
		break;

	case EGrappleState::Firing:
		Tick_GrappleFiring();
		break;

	case EGrappleState::NearingTarget:
		Tick_GrappleNearingTarget();
		break;

	case EGrappleState::OnTarget:
		Tick_GrappleOnTarget();
		break;

	default:
		break;
	}
}



