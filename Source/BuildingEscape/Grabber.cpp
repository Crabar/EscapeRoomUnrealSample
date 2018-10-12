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
	if (!PhysicsHandle)
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
	auto GrabTarget = GetGrabTarget();
	if (PhysicsHandle && GrabTarget.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(GrabTarget.GetComponent(), GrabTarget.BoneName, GrabTarget.GetActor()->GetActorLocation());
	}	
}

void UGrabber::OnReleasePressed()
{
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetReachStartPoint()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return  PlayerViewPointLocation;
}

FVector UGrabber::GetReachEndPoint()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FHitResult UGrabber::GetGrabTarget()
{
	auto LineTraceStart = GetReachStartPoint();
	auto LineTraceEnd = GetReachEndPoint();
	DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor(255, 0, 0), false, -1, 0, 3);
	FHitResult HitResult;
	const auto CollisionQueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	const auto IsHitted = GetWorld()->LineTraceSingleByObjectType(OUT HitResult, LineTraceStart, LineTraceEnd, FCollisionObjectQueryParams(ECC_PhysicsBody), CollisionQueryParams);
	if (IsHitted)
	{		
		UE_LOG(LogCollision, Warning, TEXT("I'm looking at %s"), *HitResult.GetActor()->GetName());
	}
	return HitResult;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		auto LineTraceEnd = GetReachEndPoint();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}