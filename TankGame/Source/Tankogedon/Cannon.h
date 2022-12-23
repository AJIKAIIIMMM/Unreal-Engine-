// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Tankogedon/GameStruct.h>
#include "Projectile.h"
#include "Burntile.h"
#include "Cannon.generated.h"


UCLASS()
class TANKOGEDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	virtual void Tick(float DeltaTime) override;

	void Fire();
	void FireSpecial();
	void AutomaticFire();
	void CallAutomaticFire();
	bool IsReadyToFire();

	void ReduceProjectile();
	void ReduceHeavyBullets();
	void AddProjectiles();
	void AddBurntiles();
	void AddHeavyBullets();

	
protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	uint8 Projectiles = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	uint8 Burntiles = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	uint8 HeavyBullets = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<ABurntile> BurntileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float AutomateFireRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float BulletsAmount = 2.0f;

	int8 RepeatingCallsRemaining = 3;
	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
};
