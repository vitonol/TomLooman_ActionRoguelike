// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);


UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;

	APlayerCameraManager* Camera;

	void Tick(float DeltaSeconds) override;

	UPROPERTY(Blueprintable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BPBEginPlayeingState();
	
};
