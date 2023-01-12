// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "Components\StaticMeshComponent.h"
#include "Cannon.h"
#include <Tankogedon/TankPawn.h>

// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMesh->SetupAttachment(SceneComp);
	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AmmoMesh->SetCollisionProfileName("OverlapAll");
	AmmoMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);

}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		if (TankPawn->changeFlag)
		{
			TankPawn->SecondCannonClass = CannonClass;
			TankPawn->SetupCannon(TankPawn->SecondCannonClass);
			TankPawn->changeFlag = true;
			Destroy();
		}
		else
		{
			TankPawn->CannonClass = CannonClass;
			TankPawn->SetupCannon(TankPawn->CannonClass);
			TankPawn->changeFlag = false;
			Destroy();
		}
		ACannon* Cannon = TankPawn->GetCannon();
		if (Cannon) Cannon->AddAmmo(Ammo);
		Destroy();
	}
}


