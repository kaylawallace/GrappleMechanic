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
	if (NewTarget == CurrentTarget)
	{
		return;
	}

	CurrentTarget = NewTarget;
}

void UGrappleBehaviourComponent::TryGrapple()
{
	if (GrappleState != EGrappleState::Retracted || !CurrentTarget)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Exit state for Try Grapple"));

		return;
	}

	FTransform SpawnTransform = FTransform(FRotator(0.f), OwningCharacter->GetActorLocation(), FVector(1.f));

	//AGrappleEndPoint* EndPoint = GetWorld()->SpawnActorDeferred<AGrappleEndPoint>(
	//	GrappleEndPointClass,
	//	SpawnTransform, 
	//	OwningCharacter, 
	//	nullptr, 
	//	ESpawnActorCollisionHandlingMethod::AlwaysSpawn, 
	//	ESpawnActorScaleMethod::SelectDefaultAtRuntime
	//	);

	EndPoint = Cast<AGrappleEndPoint>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		GetWorld(),
		GrappleEndPointClass, 
		SpawnTransform, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		OwningCharacter
	));

	EndPoint->SetGrappleTarget(CurrentTarget);

	UGameplayStatics::FinishSpawningActor(EndPoint, SpawnTransform);

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
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Hit actor, %s, is valid"), *Hit.GetActor()->GetName()));

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
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, BestTarget->GetName());*/

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
	if (EndPoint->GetDistanceToTarget() < StartGrappleDistance)
	{
		OwningCharacter->LaunchCharacter(FVector(0.f, 0.f, 300.f), true, true);

		GrappleState = EGrappleState::NearingTarget;
	}
}

void UGrappleBehaviourComponent::Tick_GrappleNearingTarget()
{
	if (OwningCharacter->GetVelocity().Z <= 0.f)
	{
		InitLaunchDirection = (CurrentTarget->GetCharacterLandingLocation() - OwningCharacter->GetActorLocation()).GetSafeNormal();
		//OwningCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GrappleState = EGrappleState::OnTarget;
	}
}

void UGrappleBehaviourComponent::Tick_GrappleOnTarget()
{
	if (OwningCharacter->GetDistanceTo(CurrentTarget) < EndGrappleDistance)
	{
		FVector LaunchVelocity = InitLaunchDirection * 400.f; 
		LaunchVelocity.Z = FMath::Max(LaunchVelocity.Z, 600.f);

		OwningCharacter->LaunchCharacter(LaunchVelocity, true, (OwningCharacter->GetVelocity().Z > 0.f) /*true*/);

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

	if (GEngine)
	{
		if (GrappleCable)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf((TEXT("Grapple Cable = %s")), *FString(GrappleCable->GetName())));
		}
		else 
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("No valid Grapple Cable"));
		}

		if (EndPoint)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf((TEXT("Grapple End Point = %s")), *FString(EndPoint->GetName())));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("No valid Grapple End Point"));
		}
	}
		

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
	}
}



