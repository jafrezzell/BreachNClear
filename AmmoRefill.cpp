// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoRefill.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "ShooterCharacter.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoRefill::AAmmoRefill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(Root);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAmmoRefill::OnCollision);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	SpotLight->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AAmmoRefill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoRefill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoRefill::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	AShooterCharacter* ValidShooter = Cast<AShooterCharacter>(OtherActor);

	if (!ValidShooter || !ValidShooter->GetName().Contains(TEXT("Player")))
		return;

	bool bDidIncreaseAmmo = false;

	for (AGun* Gun : ValidShooter->GetWeaponArray())
	{
		if (Gun && Gun->GetCurrentAmmo() < Gun->GetMaxAmmo())
		{
			bDidIncreaseAmmo = true;
			Gun->IncreaseAmmo(Gun->GetMaxAmmo());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
		}
	}
	if (bDidIncreaseAmmo)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

