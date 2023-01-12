// Fill out your copyright notice in the Description page of Project Settings.

#include "HeavybulletsAmmoBox.h"
#include "Components\StaticMeshComponent.h"
#include "Cannon.h"
#include <Tankogedon/TankPawn.h>


void AHeavybulletsAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		TankPawn->AddHeavyBullets();
		Destroy();
	}
}
