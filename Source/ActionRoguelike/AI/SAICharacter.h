// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
/* Pawn Sensing VS AI Perception:
 * To give sense of sight and hearing to AI
 * PawnSensing is easier to use, more capable
 *
 */

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
