// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharacterEffect, Hit.Location, ShotDirection.Rotation());
			//UE_LOG(LogTemp, Error, TEXT("Distance was %f"), Hit.Distance);

			float OverallGunDamage = Damage;
			if (Hit.Distance > MaxRange)
			{
				OverallGunDamage -= FMath::Clamp(OverallGunDamage, 1.0f, FMath::Abs((Hit.Distance / MaxRange) * DamageFallOff));
			}
			FPointDamageEvent DamageEvent(OverallGunDamage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(OverallGunDamage, DamageEvent, OwnerController, this);
		}

		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
	}
	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) 
		return false;
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * LineTraceMaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return nullptr;
	return OwnerPawn->GetController();
}

void AGun::SetGunActive(bool bToHide)
{
	SetActorEnableCollision(bToHide);
	SetActorTickEnabled(bToHide);
	SetActorHiddenInGame(!bToHide);
}

void AGun::SubtractAmmo(int Amount)
{
	CurrentAmmo -= Amount;
}

void AGun::IncreaseAmmo(int Amount)
{
	CurrentAmmo += FMath::Clamp(Amount, 0, MaxAmmo - CurrentAmmo);
}

float AGun::GetDamageFallOff() const
{
	return DamageFallOff;
}

float AGun::GetFireRate() const
{
	return FireRate;
}

int AGun::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int AGun::GetMaxAmmo() const
{
	return MaxAmmo;
}
