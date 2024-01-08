// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleBehaviourComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <GrappleTarget.h>
#include "Engine.h"

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
	if (NewTarget == CurrentTarget)
	{
		return;
	}

	CurrentTarget = NewTarget;
}

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
		//if (/*OutActor->GetClass() != TSubclassOf<AGrappleTarget>()*/ !OutActor->IsA(AGrappleTarget::StaticClass()))
		//{
		//	if (GEngine)
		//		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, OutActor->GetClass()->GetName());
		//	continue;
		//}

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, OutActor->GetClass()->GetName());

		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		FCollisionResponseParams ResponseParams;

		// Check nothing is blocking potential sight to current Grapple Target
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			OwningCharacter->GetActorLocation(),
			OutActor->GetActorLocation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			Hit,
			true,
			FColor::Red,
			FColor::Green,
			0.f
		);



		// Best Target Calculation
		if (Hit.GetActor())
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Hit actor, %s, is valid"), *Hit.GetActor()->GetName()));

			if (Hit.GetActor() == OutActor)
			{
				FVector Dir = OutActor->GetActorLocation() - OwningCharacter->GetActorLocation();
				FVector NormalizedDir = Dir.GetSafeNormal();
				FVector CameraForward = OwningCharacter->GetFollowCamera()->GetForwardVector();

				double Dot = NormalizedDir.Dot(CameraForward);
				float CurrentAngle = acos(Dot) * (180 / PI);

				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Purple, FString::Printf(TEXT("Current Angle = %f, Smallest Angle = %f"), CurrentAngle, SmallestAngle));

				

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
		/*FVector Origin;
		FVector Bounds;

		BestTarget->GetActorBounds(true, Origin, Bounds);

		DrawDebugSphere(
			GetWorld(),
			BestTarget->GetActorLocation(),
			Bounds.Size(),
			4,
			FColor::Magenta,
			false,
			0.f,
			0,
			2.f
		);*/

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, BestTarget->GetName());

		SetCurrentTarget(Cast<AGrappleTarget>(BestTarget));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("No best target found"));
	}
}

void UGrappleBehaviourComponent::Tick_GrappleFiring()
{
}

void UGrappleBehaviourComponent::Tick_GrappleNearingTarget()
{
}

void UGrappleBehaviourComponent::Tick_GrappleOnTarget()
{
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

	if (CurrentTarget)
	{
		FVector Origin;
		FVector Bounds;

		CurrentTarget->GetActorBounds(true, Origin, Bounds);

		DrawDebugSphere(
			GetWorld(),
			CurrentTarget->GetActorLocation(),
			Bounds.Size(),
			4,
			FColor::Magenta,
			false,
			0.f,
			0,
			2.f
		);

		

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, BestTarget->GetName());

		//SetCurrentTarget(Cast<AGrappleTarget>(BestTarget));
	}

	//DrawDebugLine(
	//	GetWorld(),
	//	OwningCharacter->GetActorLocation(),
	//	(OwningCharacter->GetActorLocation() + OwningCharacter->GetFollowCamera()->GetForwardVector() * 600.f),
	//	FColor::Orange,
	//	false,
	//	0.f,
	//	0,
	//	2.f
	//);
}



