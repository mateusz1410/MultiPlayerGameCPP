// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Shooter), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERGAMECPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	int32 Capacity;

public:	
	UPROPERTY(Replicated,VisibleDefaultsOnly, BlueprintReadOnly, Category = "Items")
	TArray<class APickupableItem*> PlayerItems;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddPlayerItem(APickupableItem* Item);

	FORCEINLINE TArray<class APickupableItem*> GetPlayerIntems() { return PlayerItems;}


	FORCEINLINE void SetCapacity(int32 NewCapacity) {Capacity = NewCapacity;}
	FORCEINLINE int32 GetCapacity() { return Capacity; }
		
	UFUNCTION(BlueprintCallable, Category = "Player")
	UTexture2D* GetItemIcon(int32 Index);
};
