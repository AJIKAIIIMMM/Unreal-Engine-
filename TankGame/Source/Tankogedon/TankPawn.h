

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;
	
	void MoveForward(float ForwardValue);
	void MoveRight(float RightValue);
	void RotateRight(float RotateValue);

	void Fire();
	void FireSpecial();
	void AutomaticFire();

	void ChangeCannon();
	void AddProjectiles();
	void AddBurntiles();
	void AddHeavyBullets();


	void SetupCannon(TSubclassOf<ACannon> newCannonClass);

	virtual void TakeDamage(FDamageData DamageData);

	UFUNCTION()
	void DamageTaken(float Value);

	UFUNCTION()
	void Die();

	bool changeFlag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> SecondCannonClass;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY()
	ACannon* Cannon;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurretRotationInterpolationKey = 0.1f;

	UPROPERTY()
	class ATankPlayerController* TankController;

	
private:
	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f;
	float targetRotateRightAxisValue = 0.0f;

	

	void MovementAndRotation(float DeltaTime);
};
