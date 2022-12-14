// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("AutomaticFire", EInputEvent::IE_Pressed, this, &ATankPlayerController::AutomaticFire);
	InputComponent->BindAction("ChangeCannon", EInputEvent::IE_Pressed, this, &ATankPlayerController::ChangeCannon);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	if (!TankPawn) return;
	FVector PawnPosition = TankPawn->GetActorLocation();
	MousePos.Z = PawnPosition.Z;
	FVector dir = MousePos - PawnPosition;
	dir.Normalize();
	MousePos = PawnPosition + dir * 1000;
	// DrawDebugLine(GetWorld(), PawnPosition, MousePos, FColor::Green, false, 0.1f, 0, 5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float ForwardValue)
{
	if(TankPawn)
	{
		TankPawn->MoveForward(ForwardValue);
	}
}

void ATankPlayerController::MoveRight(float RightValue)
{
	if (TankPawn)
	{
		TankPawn->MoveRight(RightValue);
	}
}

void ATankPlayerController::RotateRight(float RotateValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(RotateValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::AutomaticFire()
{
	if (TankPawn)
	{
		TankPawn->AutomaticFire();
	}
}

void ATankPlayerController::ChangeCannon()
{
	if (TankPawn)
	{
		TankPawn->ChangeCannon();
	}
}
