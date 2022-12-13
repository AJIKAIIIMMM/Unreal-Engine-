// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireTrace = 0 UMETA(DisplayName = "Use trace"),
	FireProjectile = 1 UMETA(DisplayName = "Use projectile"),
	FireLaser = 2 UMETA(DisplayName = "Use laser"),
	FireHeavyBullet = 3 UMETA(DisplayName = "Use heavy bullet")
};


UCLASS()
class TANKOGEDON_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};