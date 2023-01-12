

#include "Cannon.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Tankogedon/DamageTaker.h>

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	
}

void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Projectiles == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
			FString::Printf(TEXT("NOT ENOUGH PROJECTILES TO SHOOT")));
	}
	else if (HeavyBullets == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
			FString::Printf(TEXT("NOT ENOUGH HEAVY BULLETS TO SHOOT")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
			FString::Printf(TEXT("Projectiles: %u Heavy Bullets: %u FireTraces: %u"), Projectiles, HeavyBullets, FireTraces));
	}
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire Projectile");
		if (ProjectilePool)
		{
			ProjectilePool->GetProjectile(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		}
		else
		{
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->Start();
			}
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, FireRate, false);
		bReadyToFire = false;
	}
	else if(CannonType == ECannonType::FireTrace)
	{
		AActor* owner = this; // ACannon
		AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

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
				IDamageTaker* damageActor = Cast<IDamageTaker>(hitResult.GetActor());
				if (damageActor)
				{
					FDamageData damageData;
					damageData.DamageMaker = this;
					damageData.DamageValue = Damage;
					damageData.Instigator = GetOwner();

					damageActor->TakeDamage(damageData);
				}
				else
				{
					hitResult.GetActor()->Destroy();
				}
				//AActor* OverlappedActor = hitResult.GetActor();
				//UE_LOG(LogTemp, Warning, TEXT("Actor: %s. "), *OverlappedActor->GetName());
				//OverlappedActor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceFireTrace, FireRate, false);
		bReadyToFire = false;
		if(FireTraces == 0) bReadyToFire = false;
	}
	else if(CannonType == ECannonType::FireBurntile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire Burntile");
		ABurntile* burntile = GetWorld()->SpawnActor<ABurntile>(BurntileClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (burntile)
		{
			burntile->Start();
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, FireRate, false);
		bReadyToFire = false;
	}
	else if(CannonType == ECannonType::FireBurntile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire Burntile");
		ABurntile* burntile = GetWorld()->SpawnActor<ABurntile>(BurntileClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (burntile)
		{
			burntile->Start();
		}
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceProjectile, FireRate, false);
		bReadyToFire = false;
	}
	if (Projectiles == 0) {
		bReadyToFire = false;
		// GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "Fire heavy bullet");
	/*ABurntile* burntile = GetWorld()->SpawnActor<ABurntile>(BurntileClass, ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
	if (burntile)
	{
		burntile->Start();
	}*/
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::ReduceHeavyBullets, FireRate, false);
	bReadyToFire = false;
	if (HeavyBullets == 0) {
		bReadyToFire = false; 
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("NOT ENOUGH HEAVY BULLETS TO SHOOT")));
		// GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
	}
}

void ACannon::AutomaticFire()
{
	if (!IsReadyToFire())
	{
		return;
	}
	if (CannonType == ECannonType::FireProjectile)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::CallAutomaticFire, AutomateFireRate, true);
	}
	else if (CannonType == ECannonType::FireBurntile)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::CallAutomaticFire, AutomateFireRate, true);
	}
	else if (CannonType == ECannonType::FireHeavyBullet)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::CallAutomaticFire, AutomateFireRate, true);
	}
	else return;
}

void ACannon::CallAutomaticFire()
{
	if (CannonType == ECannonType::FireBurntile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire Burntile");
		ABurntile* burntile = GetWorld()->SpawnActor<ABurntile>(BurntileClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (burntile)
		{
			burntile->Start();
		}
		bReadyToFire = false;
		if (--RepeatingCallsRemaining <= 0)
		{
			GetWorldTimerManager().ClearTimer(ReloadTimer);
			RepeatingCallsRemaining = 3;
		}
		ReduceProjectile();
	}
	else if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "Fire Projectile");
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->Start();
		}
		bReadyToFire = false;
		if (--RepeatingCallsRemaining <= 0)
		{
			GetWorldTimerManager().ClearTimer(ReloadTimer);
			RepeatingCallsRemaining = 3;
		}
		ReduceProjectile();
	}
	else if (CannonType == ECannonType::FireHeavyBullet)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, "Fire heavy bullet");
		bReadyToFire = false;
		if (--RepeatingCallsRemaining <= 0)
		{
			GetWorldTimerManager().ClearTimer(ReloadTimer);
			RepeatingCallsRemaining = 3;
		}
		ReduceHeavyBullets();
	}
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire && (Projectiles > 0 || HeavyBullets > 0);
}

void ACannon::ReduceProjectile()
{
	if (Projectiles == 0) return;
	else
	{
		Projectiles--;
		bReadyToFire = true;
	}
}

void ACannon::ReduceHeavyBullets()
{
	if (HeavyBullets == 0) return;
	else
	{
		HeavyBullets--;
		bReadyToFire = true;
	}
}

void ACannon::ReduceFireTrace()
{
	if (FireTraces == 0) return;
	else
	{
		FireTraces--;
		bReadyToFire = true;
	}
}

void ACannon::AddAmmo(int32 Ammo)
{
	Projectiles += Ammo;
	HeavyBullets += Ammo;
	FireTraces += Ammo;
	Burntiles += Ammo;
}

void ACannon::CreateProjectilePool()
{
	if (ProjectilePoolClass)
	{
		ProjectilePool = GetWorld()->SpawnActor<AProjectilePool>(ProjectilePoolClass, ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bReadyToFire = true;
	CreateProjectilePool();
	
}
