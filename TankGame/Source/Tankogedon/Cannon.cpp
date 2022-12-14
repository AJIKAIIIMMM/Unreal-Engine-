

#include "Cannon.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	if(CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire Projectile");
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
																		ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->Start();
			}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, FireRate, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire Trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 1.0f, 0, 5);
			if (hitResult.GetActor())
			{
				AActor* OverlappedActor = hitResult.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s. "), *OverlappedActor->GetName());
				OverlappedActor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, FireRate, false);
	}
	if (Projectiles == 0) {
		bReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}
	if (CannonType == ECannonType::FireLaser)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Fire laser");
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceHeavyBullets, FireRate, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "Fire heavy bullet");
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceHeavyBullets, FireRate, false);
	}
	if (HeavyBullets == 0) {
		bReadyToFire = false; 
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
}

void ACannon::AutomaticFire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	switch(CannonType)
	{
		case ECannonType::FireProjectile:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire Projectile");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, AutomateFireRate, true);
			break;
		case ECannonType::FireTrace:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire Trace");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, AutomateFireRate, true);
			break;
		case ECannonType::FireLaser:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Fire laser");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceHeavyBullets, AutomateFireRate, true);
			break;
		case ECannonType::FireHeavyBullet:
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "Fire heavy bullet");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceHeavyBullets, AutomateFireRate, true);
			break;
		default:
			return;
	}
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire && (Projectiles > 0 || HeavyBullets > 0);
}

void ACannon::Reload()
{
	Projectiles = 20;
	HeavyBullets = 10;
	bReadyToFire = true;
}

void ACannon::ReduceProjectile()
{
	if (Projectiles == 0) return;
	else Projectiles--;
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(ReloadTimer);
		RepeatingCallsRemaining = 3;
	}
}

void ACannon::ReduceHeavyBullets()
{
	if (HeavyBullets == 0) return;
	else HeavyBullets--;
	{
		GetWorldTimerManager().ClearTimer(ReloadTimer);
		RepeatingCallsRemaining = 3;
	}
}



void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bReadyToFire = true;
	
}
