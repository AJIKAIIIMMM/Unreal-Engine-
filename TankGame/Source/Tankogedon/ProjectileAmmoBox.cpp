
#include "ProjectileAmmoBox.h"
#include "Components\StaticMeshComponent.h"
#include "Cannon.h"
#include <Tankogedon/TankPawn.h>


void AProjectileAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		TankPawn->AddProjectiles();
		Destroy();
	}
}
