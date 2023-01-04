// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Tankogedon/GameStruct.h>
#include "Projectile.h"
#include "Burntile.h"
#include <Tankogedon/ProjectilePool.h>
#include "Cannon.generated.h"



UCLASS()
class TANKOGEDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
	DECLARE_EVENT_OneParam(ACannon, FChangeScore, float);
public:	
	ACannon();

	FChangeScore ScoreChanged;

	virtual void Tick(float DeltaTime) override;

	void Fire();
	void FireSpecial();
	void AutomaticFire();
	void CallAutomaticFire();
	bool IsReadyToFire();

	void FireProjectile();
	void FireTrace();
	void FireBurntile();

	void ReduceProjectile();
	void ReduceHeavyBullets();
	void ReduceFireTrace();

	void AddAmmo(int32 Ammo);

	float Score = 0.0f;

	UFUNCTION()
	void AddScore(float ScoreValue);

	void CreateProjectilePool();

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
	uint8 FireTraces = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<AProjectilePool> ProjectilePoolClass;

	UPROPERTY()
	AProjectilePool* ProjectilePool;

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
	float FireTraceDamage = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
	float BulletsAmount = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystemComponent* ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UAudioComponent* AudioEffect;

	int8 RepeatingCallsRemaining = 3;
	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
};
