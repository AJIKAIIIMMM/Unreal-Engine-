
#include "Burntile.h"
#include "Components/StaticMeshComponent.h"

ABurntile::ABurntile()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceeneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = sceeneComp;


	BurntileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BurntileMesh"));
	BurntileMesh->SetupAttachment(sceeneComp);

	BurntileMesh->OnComponentBeginOverlap.AddDynamic(this, &ABurntile::OnMeshOverlapBegin);
	BurntileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
}

void ABurntile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &ABurntile::Move, MoveRate, true, MoveRate);
}

void ABurntile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s. "), *GetName(), *OtherActor->GetName());
		OtherActor->Destroy();
		Destroy();
	}
}

void ABurntile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

