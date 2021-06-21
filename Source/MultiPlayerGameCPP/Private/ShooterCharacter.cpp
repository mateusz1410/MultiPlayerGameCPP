// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "PickupableItem.h"
#include "HealthComponent.h"
#include "Weapon.h"
#include "InventoryComponent.h"
#include "ShooterPlayerController.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshTPP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshTPP"));
	MeshTPP->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 10;


	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	MeshTPP->bCastDynamicShadow = false;			// Disallow mesh to cast dynamic shadows
	MeshTPP->CastShadow = false;
	GetMesh()->SetupAttachment(CameraComp);

	MeshTPP->bOwnerNoSee = true;
	GetMesh()->bOnlyOwnerSee = true;


	//SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapon"));
	//SK_Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	//SK_Weapon->bOnlyOwnerSee = true;

	//SK_Weapon_TPP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapon_TPP"));
	//SK_Weapon_TPP->SetupAttachment(MeshTPP, TEXT("WeaponSocket"));
	
	//SK_Weapon_TPP->bOwnerNoSee = true;

	WeaponAttachSocketName = "WeaponSocket";

	SetReplicates(true);
	SetReplicateMovement(true);

	InteractObject = nullptr;

	bInventoryIsOpen = false;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	if (HasAuthority())
	{

		//spawn default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}


	HealthComponent->OnHealthChanged.AddDynamic(this, &AShooterCharacter::OnHealthChanged);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AShooterCharacter::Interact);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AShooterCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AShooterCharacter::StopFire);

	PlayerInputComponent->BindAction("ToggleInventory", EInputEvent::IE_Pressed, this, &AShooterCharacter::ToggleInventory);

}

void AShooterCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void AShooterCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}


void AShooterCharacter::RegisterInteractObject(APickupableItem* InteractActor)
{
	InteractObject = InteractActor;
}

void AShooterCharacter::RemoveInteractObject()
{
	InteractObject = nullptr;
}

void AShooterCharacter::OnHealthChanged(UHealthComponent * OwningHealthComp, float Health, float HealthMAX, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorForwardVector(), Value);

	}
}
void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);

	}

}

void AShooterCharacter::ToggleInventory()
{
	if (IsLocallyControlled())
	{
		if (bInventoryIsOpen)
		{
			AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(GetController());
			
			if (ShooterController)
			{
				ShooterController->HideInventoryWidget();
			}
		}
		else
		{
			AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(GetController());

			if (ShooterController)
			{
				ShooterController->ShowInventoryWidget();
			}
		}

		bInventoryIsOpen = !bInventoryIsOpen;
	}

}

void AShooterCharacter::Interact()
{
	//if (IsLocallyControlled())
	//{
	//	InventoryComponent->SetCapacity(InventoryComponent->GetCapacity() - 1);
	//	UE_LOG(LogTemp, Warning, TEXT("Capacity %d"), InventoryComponent->GetCapacity());
	//}

	if (HasAuthority())
	{
		if (InteractObject != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added to inventory"));

			InteractObject->ItemInteraction();
			InventoryComponent->AddPlayerItem(InteractObject);
			//UE_LOG(LogTemp, Warning, TEXT("Server"));
		}
	}
	else
	{

		ServerInteract();
		ServerAddToInventory();
		//UE_LOG(LogTemp, Warning, TEXT("Client"));
		if (InteractObject != nullptr)
		{
			InteractObject->ItemInteraction();
			
			//UE_LOG(LogTemp, Warning, TEXT("Server"));
		}

	}

	//InteractMessage();
}

void AShooterCharacter::ServerInteract_Implementation()
{
	if (InteractObject!=nullptr)
	{
		InteractObject->ItemInteraction();
		
	}
}


void AShooterCharacter::ServerAddToInventory_Implementation()
{
	if (InteractObject != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added to inventory"));
		InventoryComponent->AddPlayerItem(InteractObject);
	}
}


bool AShooterCharacter::ServerInteract_Validate()
{
	return true;
}


bool AShooterCharacter::ServerAddToInventory_Validate()
{
	return true;
}


void AShooterCharacter::InteractMessage()
{
	FVector StringLocation = GetActorLocation();
	StringLocation.Z += 50;
	StringLocation.X += 100;
	DrawDebugString(GetWorld(), StringLocation, TEXT("INTERACTION WAS MADE"), this, FColor::Red, 55.f);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterCharacter, InteractObject);
	DOREPLIFETIME(AShooterCharacter, CurrentWeapon); //replicate CurrentWeapon  to every ASCharacter

}

