// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

#include "Net/UnrealNetwork.h"


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
	TargetPitch = 0;

	SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpend(); // need to manually call on the server
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurentPitch = LidMesh->GetRelativeRotation().Pitch;
	float SpeedMultiplier = 1.5f;
	float Target = (CurentPitch + DeltaTime*(TargetPitch - CurentPitch) * SpeedMultiplier);

	if (!FMath::IsNearlyEqual(Target, TargetPitch, 1.f)) // does not work =( 
	{
		LidMesh->SetRelativeRotation(FRotator(Target, 0 , 0)); // @TODO junk fix me
	}
}

void ASItemChest::OnRep_LidOpend()
{
	TargetPitch = bLidOpened ? 110.f : 0;
	SetActorTickEnabled(true);
}

void ASItemChest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened); // everytime lid is open send it to all clients	
}