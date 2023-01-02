
#include "TankFactory.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(BoxCollision);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaken);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle spawnTimer;
	GetWorld()->GetTimerManager().SetTimer(spawnTimer, this, &ATankFactory::SpawnNewTank, 
		spawnTankRate, true, spawnTankRate);
}

void ATankFactory::DamageTaken(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank factory damage: %f, health: %f"), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::Die()
{
	Destroy();
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(),
		TankSpawnPoint->GetComponentLocation(),FVector(1));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(TankSpawnClass, spawnTransform, 
		this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewTank->SetPatrollingPath(TankWayPoints);

	UGameplayStatics::FinishSpawningActor(NewTank, NewTank->GetTransform());
}

