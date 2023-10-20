// Fill out your copyright notice in the Description page of Project Settings.


#include "SFlyingObject.h"

// Sets default values
ASFlyingObject::ASFlyingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// Mesh->bUseDefaultCollision(true);
	   
}

// Called when the game starts or when spawned
void ASFlyingObject::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASFlyingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float X = FMath::RandRange(.5f, 0.1f);
	
	FVector Loc = Mesh->GetComponentLocation() + FVector(X, FMath::Min(FMath::Sin(X), X-.01f), 0.f);
	
	bool B = FMath::RandBool();
	Loc = B ? Loc : Loc-Loc;

	float G = FMath::RandRange(-.1f, 0.1f);
	// FMath::ApplyScaleToFloat(G, Loc*0.1f);
	FRotator Rot = B ? FRotator(G) : FRotator(FMath::Cos(-G));
	
	
	Mesh->AddWorldRotation(Rot);
	
	
	// Mesh->SetWorldLocationAndRotation(Loc, Rot);
	
}

