// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName("PhysicsActor");

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(Mesh);
	RadialForce->SetAutoActivate(false);
	
	RadialForce->Radius = 1000.f;
	RadialForce->ImpulseStrength = 2500.f;
	RadialForce->bImpulseVelChange = true;

	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForce->Falloff = RIF_Constant;
}


void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Mesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("Impulse applied"));

	// %s = string
	// %f = float
	
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds); // need *before GetName() to convert type from Fstring to CharArr

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}


