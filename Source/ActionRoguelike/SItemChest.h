// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInteractibleInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISInteractibleInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* InstigatorPawn) override; // Because Interact is BlueprintNativeEvent it requires _Implementation
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpend", BlueprintReadOnly) // RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpend();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere)
	float TargetPitch;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
