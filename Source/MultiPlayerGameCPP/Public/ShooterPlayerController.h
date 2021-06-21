// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAMECPP_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

	void ShowInteractWidget();

	void HideInteractWidget();

	void ShowInventoryWidget();

	void HideInventoryWidget();

protected:

	virtual void BeginPlay() override;

	TSubclassOf<class UUserWidget> InteractWidgetClass;

	UUserWidget* InteractWidget;



	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	UUserWidget* InventoryWidget;


};
