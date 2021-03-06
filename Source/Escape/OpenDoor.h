// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor();
	void CloseDoor();
	float TotalMassOfActors() const;
	void CheckPressurePlate() const;
	void CheckAudioComponent() const;

private:

	UPROPERTY(EditAnywhere)
	float OpenTargetYaw = 90.f;
	float CloseTargetYaw = 0.f;
	float InitialYaw;
	float DoorSpeed = 4.f;
	
	UPROPERTY(EditAnywhere)
	float MassThreshold = 60.f;
	UPROPERTY(EditAnywhere)
	float DoorLastOpen = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	bool bPlayDoorSound = true;

};
