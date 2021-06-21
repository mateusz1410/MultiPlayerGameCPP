// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterDestructibleActor.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DestructibleComponent.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
// Sets default values
AShooterDestructibleActor::AShooterDestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	RootComponent = TriggerComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));
	DestructibleComponent->SetupAttachment(Mesh);
	DestructibleComponent->SetNotifyRigidBodyCollision(true); //simulate Generate Hit event  = true

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AShooterDestructibleActor::OnHealthChanged); // OnHealthChanged works only on server in HealthComponent


	bIsDestroyed = false;
	DefaultImpulse = 1;
	bIsTriggerEnabled = false;


	SetReplicates(true);
	SetReplicateMovement(true);
}

void AShooterDestructibleActor::Damage(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("DAMAGE"));


}

void AShooterDestructibleActor::Trigger(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Trigger"));
}

void AShooterDestructibleActor::Destroy()
{

}

// Called when the game starts or when spawned
void AShooterDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	//DestructibleComponent->OnComponentHit.AddDynamic(this, &AShooterDestructibleActor::Damage);
	//TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AShooterDestructibleActor::Trigger);
}

void AShooterDestructibleActor::OnHealthChanged(UHealthComponent * OwningHealthComp, float Health, float HealthMAX, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (HasAuthority())
	{
		FString HealthString = FString::SanitizeFloat(Health);
		DrawDebugString(GetWorld(), GetActorLocation(), HealthString);
	
		if (Health <= 0.0f)
		{
			UE_LOG(LogTemp,Warning, TEXT("IsDead"));

			if (HasAuthority())
			{
				UE_LOG(LogTemp, Warning, TEXT("Authority"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("client"));
			}

			SetLifeSpan(1.f);
		}

	}
}

// Called every frame
void AShooterDestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

