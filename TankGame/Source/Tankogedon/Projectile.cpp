

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include <Tankogedon/DamageTaker.h>
#include <Tankogedon/Scorable.h>


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
	GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &AProjectile::Deactivate, DeactivateTime, false);
}

void AProjectile::Deactivate()
{
	bIsActivation = false;
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimer);
	GetWorld()->GetTimerManager().ClearTimer(MovementTimer);
	SetActorEnableCollision(false);
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
		IScorable* ScorableActor = Cast<IScorable>(OtherActor);

		float ScoreValue = 0.0f;

		if (ScorableActor)
		{
			ScoreValue = ScorableActor->GetPoints();
		}

		if (DamageActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			DamageActor->TakeDamage(damageData);

			if (OtherActor->IsActorBeingDestroyed() && ScoreValue != 0.0f)
			{
				if (OnKilled.IsBound())
				{
					OnKilled.Broadcast(ScoreValue);
				}
			}
		}
		else
		{
			UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
			else
			{
				OtherActor->Destroy();
			}
		}

		Deactivate();
		// Destroy();
	}
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}



