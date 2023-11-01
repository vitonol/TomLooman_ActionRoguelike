// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "SInteractibleInterface.h"
#include "SCharacter.h"
#include "SHelicopterSM.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

#define DEBUG 1;

UCLASS()
class ACTIONROGUELIKE_API ASHelicopterSM : public APawn, public ISInteractibleInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* InstigatorPawn) override;

public:

	ASHelicopterSM();

protected:

	UPROPERTY(EditAnywhere, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* TiltRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* TiltForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* RotateRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* Look;

	
	virtual void BeginPlay() override;

	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(BlueprintReadWrite)
	FName TailRotorSocket;

	UPROPERTY(BlueprintReadWrite)
	FName RotorSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshRotor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshTailRotor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshTailBlade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshBlade;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* Springarm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
private:
	
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

	const float TurnSpeed = 0.3f;

	const float BlurBladeSpeedThreshold = 900.f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* LiftCurve;

	void TickUpdate(float Delta);

	void RotateBlades(float Delta);

	bool bAutoDown;

	float GetTurnSpeed();

	float GetCurrentLift();

	void UpdatePreviousValues();


protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void LoadBlurryBladeMesh();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void LoadNormalBladeMesh();

	void Climb(const FInputActionInstance& Instance);

	void MoveRight(const FInputActionInstance& Instance);

	void MoveForward(const FInputActionInstance& Instance);

	void RotateYaw(const FInputActionInstance& Instance);

	void LookAround(const FInputActionInstance& Instance);

	void SetBladeRotationSpeed(float Value, float DeltaTime);

	
public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
