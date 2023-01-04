
#include "MachinePawn.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "HealthComponent.h"

AMachinePawn::AMachinePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"), false);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMachinePawn::DamageTaken);
	HealthComponent->OnDie.AddUObject(this, &AMachinePawn::Die);
}

void AMachinePawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AMachinePawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void AMachinePawn::AutomaticFire()
{
	if (Cannon)
	{
		Cannon->AutomaticFire();
	}
}

void AMachinePawn::ChangeCannon()
{
	if (!changeFlag)
	{
		SetupCannon(SecondCannonClass);
		changeFlag = true;
	}
	else
	{
		SetupCannon(CannonClass);
		changeFlag = false;
	}
}

void AMachinePawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->SetOwner(this);
	Cannon->ScoreChanged.AddUObject(this, &AMachinePawn::ShowScore);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

ACannon* AMachinePawn::GetCannon() const
{
	return Cannon;
}

float AMachinePawn::GetPoints()
{
	return ScoreValue;
}

void AMachinePawn::ShowScore(float Value)
{
	Score += Value;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Score: %f"), Score));
}

void AMachinePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AMachinePawn::DamageTaken(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

void AMachinePawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

FVector AMachinePawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCannon(CannonClass);
}



