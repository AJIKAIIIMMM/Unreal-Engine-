
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "Cannon.h"
#include "MachinePawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDON_API AMachinePawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	AMachinePawn();

	void Fire();
	void FireSpecial();
	void AutomaticFire();

	void ChangeCannon();

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	ACannon* GetCannon() const;

	virtual void TakeDamage(FDamageData DamageData);

	UFUNCTION()
	void DamageTaken(float Value);

	UFUNCTION()
	void Die();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;

	bool changeFlag = false;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY()
	ACannon* Cannon;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;
};
