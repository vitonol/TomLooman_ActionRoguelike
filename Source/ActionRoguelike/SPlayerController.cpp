// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	
	
}

void ASPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode((FInputModeGameOnly()));
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::Tick(float DeltaSeconds)
{
	// Camera->AddActorLocalRotation(FRotator(10,0,0));

	Super::Tick(DeltaSeconds);
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	BPBEginPlayeingState();	
}
