// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class ATankPawn* TankPawn;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;
	FVector	GetMousePosition() { return MousePos; }
protected:
	virtual void BeginPlay() override;

	void MoveForward(float ForwardValue);
	void MoveRight(float RightValue);
	void RotateRight(float RotateValue);

	void Fire();
	void FireSpecial();
	void AutomaticFire();

	void ChangeCannon();

	FVector MousePos;
};
