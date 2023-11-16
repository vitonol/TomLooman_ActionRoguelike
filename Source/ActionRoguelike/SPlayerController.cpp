// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Kismet/GameplayStatics.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	
	
}

void ASPlayerController::Tick(float DeltaSeconds)
{
	// Camera->AddActorLocalRotation(FRotator(10,0,0));

	Super::Tick(DeltaSeconds);
}
