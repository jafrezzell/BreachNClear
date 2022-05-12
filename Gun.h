// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	virtual void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGunActive(bool bToHide);

	float GetDamageFallOff() const;

	float GetFireRate() const;

	UFUNCTION(BlueprintPure)
	int GetCurrentAmmo() const;
	
	UFUNCTION(BlueprintPure)
	int GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable)
	void SubtractAmmo(int Amount);

	UFUNCTION(BlueprintCallable)
	void IncreaseAmmo(int Amount);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitCharacterEffect;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float LineTraceMaxRange = 5000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditAnywhere)
	float DamageFallOff = 1.0;

	AController* GetOwnerController() const;

	UPROPERTY(EditAnywhere)
	float FireRate = .15f;

	UPROPERTY(EditAnywhere)
	int MaxAmmo = 25;

	int CurrentAmmo = 0;
};
