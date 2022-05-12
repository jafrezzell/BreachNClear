// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoRefill.generated.h"

class UBoxComponent;
class USpotLightComponent;

UCLASS()
class SIMPLESHOOTER_API AAmmoRefill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoRefill();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* SpotLight;

	UPROPERTY(EditAnywhere)
	USoundBase* PickUpSound;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
