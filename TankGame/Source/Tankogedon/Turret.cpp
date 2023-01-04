

#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include <Components\ArrowComponent.h>
#include <Engine/StaticMesh.h>
#include "HealthComponent.h"
#include "UObject/Object.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimer;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimer, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
	GetWorld()->GetTimerManager().SetTimer(ChangeWeaponTimer, this, &AMachinePawn::ChangeCannon, TimeToChangeWeapon, true);
}

void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

void ATurret::Targeting()
{
	
	if (!PlayerPawn)
	{
		return;
	}
	if (IsPlayerInRange())
	{
		RotateToPlayer();

		if (CanFire())
		{
			Fire();
		}
	}
}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	if (!IsPlayerSeen())
	{
		return false;
	}
	FVector targetDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();

	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetDir, DirToPlayer)));
	return aimAngle <= Accurency;
}

bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = this->GetEyesPosition();

	FHitResult hitResult;
	FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	params.bTraceComplex = true;
	params.AddIgnoredActor(this);
	params.bReturnPhysicalMaterial = false;


	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, params))
	{
		if (hitResult.GetActor())
		{
			if (hitResult.GetActor() == PlayerPawn)
			{
				DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Green, false, 0.5f, 0, 5);
			}
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Purple, false, 0.5f, 0, 5);
			return hitResult.GetActor() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
	return false;
}
