// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "TankAIController.generated.h"

UCLASS()
class TANKOGEDON_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATankPawn* TankPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FVector> PatrollingPoints;

	UPROPERTY()
	float MovementAccurency = 0.0f;

	int32 CurrentPatrollingIndex = 0;
	
};
