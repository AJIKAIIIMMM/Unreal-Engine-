

#pragma once

#include "CoreMinimal.h"
#include "MachinePawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDON_API ATankPawn : public AMachinePawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;
	
	void MoveForward(float ForwardValue);
	void MoveRight(float RightValue);
	void RotateRight(float RotateValue);
	
	UFUNCTION()
	TArray<FVector> GetPatrollingPath() const { return PatrollingPath; }
	UFUNCTION()
	float GetMovementAccurency() const { return MovementAccurency; }
	UFUNCTION()
	FVector GetTurretForwardVector() const;
	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MovementAccurency = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

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
