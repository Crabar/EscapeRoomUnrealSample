// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define  OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::HandlePhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle included in %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::HandleInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::OnGrabPressed);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::OnReleasePressed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input component included in %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::HandleComponents()
{
	HandlePhysicsComponent();
	HandleInputComponent();
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	HandleComponents();
}

void UGrabber::OnGrabPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("GRAB!"));
	auto GrabTarget = GetGrabTarget();
	if (GrabTarget.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(GrabTarget.GetComponent(), GrabTarget.BoneName, GrabTarget.GetActor()->GetActorLocation());
	}	
}

void UGrabber::OnReleasePressed()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("RELEASE!"));
}

const FHitResult UGrabber::GetGrabTarget()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	const auto LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, -1, 0, 3);
	FHitResult HitResult;
	const auto CollisionQueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	const auto IsHitted = GetWorld()->LineTraceSingleByObjectType(OUT HitResult, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECC_PhysicsBody), CollisionQueryParams);
	if (IsHitted)
	{		
		UE_LOG(LogCollision, Warning, TEXT("I'm looking at %s"), *HitResult.GetActor()->GetName());
	}
	return HitResult;
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		const auto LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

