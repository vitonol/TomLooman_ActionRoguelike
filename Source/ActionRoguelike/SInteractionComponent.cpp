// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SInteractibleInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool>CVarDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable debug lines for Interacft Component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
	
	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	float Radius = 50.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit? FColor::Green : FColor::Red;
	
	for (auto& Hit  : Hits)
	{
		if (bDebugDraw)
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.f);
		  
		if (AActor* HitActor = Hit.GetActor()) // to avoid calling functions on nullptr
		{
			if (HitActor->Implements<USInteractibleInterface>()) // Needs "U"
			{
				APawn* MyPawn = Cast<APawn>(MyOwner); // safe casting, no need to check for nullptr
				ISInteractibleInterface::Execute_Interact(HitActor, MyPawn);
				break; 
			}
		}

	}  
	if (bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation,End, LineColor, false, 2.f, 0, 1.5f );
}