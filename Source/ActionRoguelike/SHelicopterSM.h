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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterChopper);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitChopper);


UENUM()
enum class EChoppperState : uint32
{
	Off = 0,
	Startup = 1,
	Idle = 2,
	Flying = 3
};

UCLASS()
class ACTIONROGUELIKE_API ASHelicopterSM : public APawn, public ISInteractibleInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* InstigatorPawn) override;

public:

	ASHelicopterSM();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	EChoppperState ChopperState = EChoppperState::Off;
		
	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnterChopper OnPlayerEnterChopper;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerExitChopper OnPlayerExitChopper;

	//~ Begin APawn Interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	//~ End APawn Interface

	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface

protected:

	void OnConstruction(const FTransform& Transform) override;

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBladeRotationSpeed();

	FVector Velocity;
	FVector Acceleration;
	FVector PrevVelocity;
	FVector PrevAcceleration;
	FVector PrevPosition;

	FVector ForwardAxis;

	FTransform WorldTransform;

	float CurrentTurn;
	float CurrentPitch;
	float CurrentRotation;
	float ForwardSpeed;
	
	const float MaxBladeRotationSpeed = 1600.f;

	const float ThrottleUpSpeed = 300.f;

	const float TurnSpeed = 0.3f;

	const float BlurBladeSpeedThreshold = 900.f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* LiftCurve;

	void TickUpdate(float Delta);

	void RotateBlades(float Delta);

	bool bAutoDown;

	bool bStartingUp;

	float GetTurnSpeed();

	float GetCurrentLift();

	void UpdatePreviousValues();

	void UpdateState(float Delta);

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
	
};
