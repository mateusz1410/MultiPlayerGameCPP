// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupableItem.generated.h"

UCLASS()
class MULTIPLAYERGAMECPP_API APickupableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* GrabSphere;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCompBeginOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnCompEndOverlapPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* CollectParticleSystem;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Icon")
	class UTexture2D* ItemIcon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ItemInteraction();

	UFUNCTION(NetMulticast,Unreliable)
	void ServerPickedUpParticleEffect();

	FORCEINLINE UTexture2D* GetItemIcon()const {return ItemIcon;}
};
