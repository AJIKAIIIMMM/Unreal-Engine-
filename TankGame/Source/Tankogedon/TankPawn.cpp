
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


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

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
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator TurretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = TurretRotation.Pitch;
		targetRotation.Roll = TurretRotation.Roll;

		FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);
		// UE_LOG(LogTemp, Warning, TEXT("targetRotation: %s, LerpRotation: %s"), *targetRotation.ToString(), *newTurretRotation.ToString());

		TurretMesh->SetWorldRotation(newTurretRotation);
	}
	if (Cannon->Projectiles == 0 && Cannon->HeavyBullets == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
			FString::Printf(TEXT("YOUR AMMO IS EMPTY PRESS 'R' TO RELOAD")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
			FString::Printf(TEXT("Projectiles: %u Heavy Bullets: %u"), Cannon->Projectiles, Cannon->HeavyBullets));
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

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::AutomaticFire()
{
	if (Cannon)
	{
		Cannon->AutomaticFire();
	}
}

void ATankPawn::Reload()
{
	if (Cannon)
	{
		Cannon->Reload();
	}
}

void ATankPawn::ChangeCannon()
{
	InterimCannon = CannonClass;
	SetupCannon(SecondCannonClass);
	SecondCannonClass = InterimCannon;
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if(Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(CannonClass);
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