

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = sceeneComp;


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	// ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this,&AProjectile::Move, MoveRate, true, MoveRate);

}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s. "), *GetName(), *OtherActor->GetName());
		OtherActor->Destroy();
		Destroy();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}



