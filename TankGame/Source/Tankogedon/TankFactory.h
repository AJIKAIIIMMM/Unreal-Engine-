
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "Engine/TargetPoint.h"
#include "TankFactory.generated.h"

class AMapLoader;
UCLASS()
class TANKOGEDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	ATankFactory();

	virtual void TakeDamage(FDamageData DamageData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	AMapLoader* LinkedMapLoader;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DamageTaken(float DamageValue);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void SpawnNewTank();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TSubclassOf<class ATankPawn> TankSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	float spawnTankRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TArray<ATargetPoint*> TankWayPoints;
};
