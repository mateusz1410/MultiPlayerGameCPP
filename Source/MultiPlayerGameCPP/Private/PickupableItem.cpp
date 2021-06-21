// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupableItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ShooterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ShooterPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"


// Sets default values
APickupableItem::APickupableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	SetRootComponent(GrabSphere);
	GrabSphere->InitSphereRadius(100);

	GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupableItem::OnCompBeginOverlapPickup);

	GrabSphere->OnComponentEndOverlap.AddDynamic(this, &APickupableItem::OnCompEndOverlapPickup);


	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	SetReplicates(true);

}

// Called when the game starts or when spawned
void APickupableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupableItem::OnCompBeginOverlapPickup(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{

		AShooterCharacter* Shooter = Cast<AShooterCharacter>(OtherActor);

		if (Shooter->IsLocallyControlled())
		{
			//Shooter->InteractMessage();
			AShooterPlayerController* PC = Cast<AShooterPlayerController>(Shooter->GetController());
			if (PC)
			{
				PC->ShowInteractWidget();
			}
		}

		if (HasAuthority())
		{
			if (Shooter)
			{

				Shooter->RegisterInteractObject(this);

				//UE_LOG(LogTemp, Warning, TEXT("BeginOveralpPickup %s"), *OtherComp->GetName());
			}

		}


	}

}

void APickupableItem::OnCompEndOverlapPickup(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

		AShooterCharacter* Shooter = Cast<AShooterCharacter>(OtherActor);
		if (Shooter->IsLocallyControlled())
		{
			//Shooter->InteractMessage();
			AShooterPlayerController* PC = Cast<AShooterPlayerController>(Shooter->GetController());
			if (PC)
			{
				PC->HideInteractWidget();
			}
		}

		if (GetLocalRole() == ROLE_Authority)
		{
			if (Shooter)
			{

				Shooter->RemoveInteractObject();
				//UE_LOG(LogTemp, Warning, TEXT("EndOveralpPickup %s"), *OtherComp->GetName());
			}

		}
	}
}



void APickupableItem::ItemInteraction()
{

	//	FVector StringLocation = GetActorLocation();
	//	StringLocation.Z += 50;
	//	StringLocation.X += 100;

		//DrawDebugString(GetWorld(), StringLocation, TEXT("Interaction was made"), this, FColor::Red, 55.f);
		ServerPickedUpParticleEffect();
		//UE_LOG(LogTemp, Warning, TEXT("done interact Server"));



		SetLifeSpan(1.f);


}
void APickupableItem::ServerPickedUpParticleEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectParticleSystem, GetActorLocation(), FRotator::ZeroRotator);
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN PARTICLE EFFECT"));
}