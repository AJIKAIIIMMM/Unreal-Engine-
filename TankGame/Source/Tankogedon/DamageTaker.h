
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <Tankogedon/GameStruct.h>
#include "DamageTaker.generated.h"


UINTERFACE(MinimalAPI)
class UDamageTaker : public UInterface
{
	GENERATED_BODY()
};


class TANKOGEDON_API IDamageTaker
{
	GENERATED_BODY()


public:
	virtual void TakeDamage(FDamageData DamageData) = 0;
};
