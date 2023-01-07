// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Particles\ParticleSystemComponent.h"
#include "DamageTaker.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
class UPhysicsComponent;
UCLASS()
class TANKOGEDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	APhysicsProjectile();

	virtual void Start() override;

protected:
	virtual void Move() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPhysicsComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* TrailEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveAccurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TrajectoryMaxTime = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TrajectoryTimeStep = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TrajectorySpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShowTrajectory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector moveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 TrajectoryCurrentIndex;

};