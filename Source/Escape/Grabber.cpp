// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandler();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandler->GrabbedComponent)
	{
		PhysicsHandler->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	const FHitResult HitResult = GetFirstPhysicsBodyInReach();

	PhysicsHandler->GrabComponentAtLocationWithRotation
	(
		HitResult.GetComponent(),
		NAME_None,
		GetLineTraceEnd(),
		GetPlayerWorldRotation()
	);
}

void UGrabber::Release()
{
	PhysicsHandler->ReleaseComponent();
}

void UGrabber::FindPhysicsHandler()
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandler)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Physics Handler Component, please add one to the actor."), *GetOwner()->GetName());	
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayerWorldPos(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Tarce Hits: %s"), *Hit.Actor->GetName());
	}

	return Hit;
}

FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetOwner()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetPlayerWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetOwner()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	return  PlayerViewPointLocation;
}

FRotator UGrabber::GetPlayerWorldRotation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetOwner()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	return  PlayerViewPointRotation;
}


