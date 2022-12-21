

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include <Tankogedon/DamageTaker.h>


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = sceeneComp;


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(sceeneComp);

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this,&AProjectile::Move, MoveRate, true, MoveRate);

}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner(); // ACannon
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr; //ATankPawn or ATurret

	/*if (Owner)
	{
		OwnerByOwner = Owner->GetOwner();
	}
	else
	{
		OwnerByOwner = nullptr;
	}*/

	if (OtherActor != owner || OtherActor != OwnerByOwner)
	{
		IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);
		if (DamageActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			DamageActor->TakeDamage(damageData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s. "), *GetName(), *OtherActor->GetName());
			OtherActor->Destroy();
		}

		
		Destroy();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}



