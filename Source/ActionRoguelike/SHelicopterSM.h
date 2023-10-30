// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "SCharacter.h"
#include "SHelicopterSM.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASHelicopterSM : public APawn
{
	GENERATED_BODY()

public:

	ASHelicopterSM();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* MoveUpp;
	
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName TailRotorSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Rotor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TailRotor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TailBlade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Blade;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* Springarm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
private:

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY()
	ASCharacter* Occupant;

	UPROPERTY()
	float TargetBladeRotationSpeed;

	UPROPERTY()
	float CurrentBladeRotationSpeed;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FVector Acceleration;

	UPROPERTY()
	FVector PrevVelocity;

	UPROPERTY()
	FVector PrevAcceleration;

	UPROPERTY()
	FVector PrevPosition;

	UPROPERTY()
	float CurrentTurn;

	UPROPERTY()
	float CurrentPitch;

	UPROPERTY()
	float CurrentRotation;

	const float MaxBladeRotationSpeed = 1500.f;

	const float ThrottleUpSpeed = 200.f;

	const float TurnSpeed = 0.1;

	const float BlurBladeSpeedThreshold = 900.f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* LiftCurve;

protected:

	void MoveUp(float Value);

	void Move(const FInputActionInstance& Instance);
	void Released(const FInputActionInstance& Instance);
public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
