
#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Runtime/Engine/Classes/Engine/World.h"
#include <Components/ArrowComponent.h>
#include "Cannon.h"
#include <Tankogedon/HealthComponent.h>


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), targetRotateRightAxisValue));
	// Movement and Rotation
	MovementAndRotation(DeltaTime);
	// Turret Rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		RotateTurretTo(mousePos);
	}
}

void ATankPawn::MoveForward(float ForwardValue)
{
	targetForwardAxisValue = ForwardValue;
}

void ATankPawn::MoveRight(float RightValue)
{
	targetRightAxisValue = RightValue;
}

void ATankPawn::RotateRight(float RotateValue)
{
	targetRotateRightAxisValue = RotateValue;
}

FVector ATankPawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;

	FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);
	// UE_LOG(LogTemp, Warning, TEXT("targetRotation: %s, LerpRotation: %s"), *targetRotation.ToString(), *newTurretRotation.ToString());

	TurretMesh->SetWorldRotation(newTurretRotation);
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
}

void ATankPawn::MovementAndRotation(float DeltaTime)
{
	// Movement
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	FVector ForwardMovement = ForwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime;
	FVector RightMovement = RightVector * MoveSpeed * targetRightAxisValue * DeltaTime;
	FVector movePosition = CurrentPosition + ForwardMovement + RightMovement;
	SetActorLocation(movePosition, true);
	// Rotation
	float yawRotation = RotationSpeed * targetRotateRightAxisValue * DeltaTime;
	float LerpRotateValue = FMath::Lerp(targetRotateRightAxisValue, LerpRotateValue, TurretRotationInterpolationKey);
	FRotator currentRotation = GetActorRotation();

	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);
}