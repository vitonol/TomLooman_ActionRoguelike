// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
	TargetPitch = 110.f;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	SetActorTickEnabled(true);
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Curent = LidMesh->GetRelativeRotation().Pitch;
	float SpeedMultiplier = 1.5f;
	float Target = (Curent + DeltaTime*(TargetPitch - Curent) * SpeedMultiplier);
	
	LidMesh->SetRelativeRotation(FRotator(Target, 0 , 0));
}

