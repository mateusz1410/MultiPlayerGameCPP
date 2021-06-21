// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/USerWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooterPlayerController::AShooterPlayerController()
{

	if (IsLocalPlayerController())
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> InterWidgetClass(TEXT("/Game/UI/WBP_Interact"));
		if (!ensure(InterWidgetClass.Class != nullptr))return;

		InteractWidgetClass = InterWidgetClass.Class;


		static ConstructorHelpers::FClassFinder<UUserWidget> InveWidgetClass(TEXT("/Game/UI/WBP_Inventory"));
		if (!ensure(InveWidgetClass.Class != nullptr))return;

		InventoryWidgetClass = InveWidgetClass.Class;

	}

}

void AShooterPlayerController::ShowInteractWidget()
{
	if (IsLocalPlayerController() && InteractWidget)
	{
		InteractWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AShooterPlayerController::HideInteractWidget()
{
	if (IsLocalPlayerController() && InteractWidget)
	{
		InteractWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AShooterPlayerController::ShowInventoryWidget()
{
	if (IsLocalPlayerController() && InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
		
		//AShooterCharacter* Shooter = Cast<AShooterCharacter>(GetPawn());
		//Shooter->GetComponentByClass(UCharacterMovementComponent::StaticClass());
		
		
	}
}

void AShooterPlayerController::HideInventoryWidget()
{
	if (IsLocalPlayerController() && InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (IsLocalPlayerController()) // local Player Controller
	{
		if (InteractWidgetClass) // Check the selected UI class is not NULL
		{
			if (!InteractWidget) // If the widget is not created and == NULL
			{

				InteractWidget = CreateWidget<UUserWidget>(this, InteractWidgetClass);

				if (!InteractWidget) return;

				InteractWidget->AddToViewport();
				InteractWidget->SetVisibility(ESlateVisibility::Hidden);

			}

		}


		if (InventoryWidgetClass) // Check the selected UI class is not NULL
		{
			if (!InventoryWidget) // If the widget is not created and == NULL
			{

				InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);

				if (!InventoryWidget) return;

				InventoryWidget->AddToViewport();
				InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

			}

		}



	}


}