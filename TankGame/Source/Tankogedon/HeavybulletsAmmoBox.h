// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "HeavybulletsAmmoBox.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDON_API AHeavybulletsAmmoBox : public AAmmoBox
{
	GENERATED_BODY()
protected:
	virtual void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
			OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult) override;
};