// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

#include "Components/AudioComponent.h"

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
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	CheckPressurePlate();
	CheckAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() >= MassThreshold)
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

void UOpenDoor::OpenDoor()
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, OpenTargetYaw, GetWorld()->GetDeltaSeconds(), DoorSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
	if (bPlayDoorSound)
	{
		AudioComponent->Play();
		bPlayDoorSound = false;
	}
}

void UOpenDoor::CloseDoor()
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw,CloseTargetYaw, GetWorld()->GetDeltaSeconds(), DoorSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
	if (!bPlayDoorSound)
	{
		AudioComponent->Play();
		bPlayDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors ;

	PressurePlate->GetOverlappingActors(OverlappingActors);
	
	
	for (auto OverlappingActor : OverlappingActors)
	{
		TArray<UPrimitiveComponent*> Components;
		OverlappingActor->GetComponents(Components);
		for (auto Component : Components)
		{
			TotalMass += Component->GetMass();
		}
	}

	return TotalMass;
}

void UOpenDoor::CheckPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Pressure Plate set."), *GetOwner()->GetName());
	}
}
void UOpenDoor::CheckAudioComponent() const
{
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Audio Component set."), *GetOwner()->GetName());
	}
}

