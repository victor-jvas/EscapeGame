// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenTargetYaw = GetOwner()->GetActorRotation().Yaw + 90;
	CloseTargetYaw = InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Pressure Plate set."), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		DoorLastOpen = GetWorld()->GetTimeSeconds();
		OpenDoor();
	}
	if (PressurePlate && !PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		if ( (GetWorld()->GetTimeSeconds() - DoorLastOpen) >= DoorCloseDelay )
		{
			CloseDoor();	
		}	
	}
}

void UOpenDoor::OpenDoor() const
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, OpenTargetYaw, GetWorld()->GetDeltaSeconds(), DoorSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor() const
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw,CloseTargetYaw, GetWorld()->GetDeltaSeconds(), DoorSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}

