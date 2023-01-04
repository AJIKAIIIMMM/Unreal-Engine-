
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Scorable.generated.h"

UINTERFACE(MinimalAPI)
class UScorable : public UInterface
{
	GENERATED_BODY()
};

class TANKOGEDON_API IScorable
{
	GENERATED_BODY()

public:
	virtual float GetPoints() = 0;
};
