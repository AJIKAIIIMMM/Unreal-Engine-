
#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "BurntileAmmoBox.generated.h"

UCLASS()
class TANKOGEDON_API ABurntileAmmoBox : public AAmmoBox
{
	GENERATED_BODY()

protected:
	virtual void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor*
			OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult) override;
};
