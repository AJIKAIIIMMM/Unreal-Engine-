
#pragma once

#include "CoreMinimal.h"
#include "MachinePawn.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDON_API ATurret : public AMachinePawn
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10.0f;
	

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
};
