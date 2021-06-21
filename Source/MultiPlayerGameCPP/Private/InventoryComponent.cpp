// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PickupableItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Capacity = 10;
	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddPlayerItem(APickupableItem * Item)
{

	PlayerItems.Add(Item);
}

UTexture2D* UInventoryComponent::GetItemIcon(int32 Index)
{
	if (!PlayerItems[Index]) return nullptr;

	return PlayerItems[Index]->GetItemIcon();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, PlayerItems);

}
