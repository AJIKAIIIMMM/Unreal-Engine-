// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAmmoBox.h"
#include "Components\StaticMeshComponent.h"
#include "Cannon.h"
#include <Tankogedon/TankPawn.h>


void AWeaponAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		if (TankPawn->changeFlag)
		{
			TankPawn->SecondCannonClass = CannonClass;
			TankPawn->SetupCannon(TankPawn->SecondCannonClass);
			TankPawn->changeFlag = true;
		}
		else
		{
			TankPawn->CannonClass = CannonClass;
			TankPawn->SetupCannon(TankPawn->CannonClass);
			TankPawn->changeFlag = false;
		}
	}
	Destroy();
}
