// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsThruster.h"
// #include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "SInteractibleInterface.h"
#include "SCopter.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UPhysicsThrusterComponent;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

// class APhysicsThruster;;

UCLASS()
class ACTIONROGUELIKE_API ASCopter : public APawn, public ISInteractibleInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;
public:
	// Sets default values for this pawn's properties
	ASCopter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// UPROPERTY(VisibleAnywhere)
 //    UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPhysicsThrusterComponent* ThrusterComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UFUNCTION()
	void MouseRight(float Value);

	UFUNCTION()
	void MouseUp(float Value);

	UFUNCTION()
	void MoveUp(float Value);

	UFUNCTION()
	void RotateRight(float Value);

	UFUNCTION()
	void TiltForward(float Value);

	UFUNCTION()
	void TiltRight(float Value);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float VarableUpForce = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float ConstantUpForce = 970;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float YawRoatationSpeed = 60;

	UFUNCTION(BlueprintSetter)
	void SetDesiredTiltAngle(float Value);

	UFUNCTION(BlueprintGetter)
	float GetDesiredTiltAngle();

	UFUNCTION(BlueprintSetter)
	void SetTiltSpeedClampBound(float Value);

	UFUNCTION(BlueprintGetter)
	float GetTiltSpeedClampBound();

	UFUNCTION(BlueprintSetter)
	void SetTiltingSpeed(float Value);

	UFUNCTION(BlueprintGetter)
	float GetTiltingSpeed();
	
private:

	UPROPERTY(EditAnywhere, BlueprintSetter = SetDesiredTiltAngle, BlueprintGetter = GetDesiredTiltAngle, 
				Category = "Helicopter Movement", meta = (ClampMin = -60, ClampMax = 60))
	float DesiredTiltAngle = 30;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltSpeedClampBound, BlueprintGetter = GetTiltSpeedClampBound,
				Category = "Helicopter Movement", meta = (ClampMin = 0))
	float TiltSpeedClampBound = 20;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetTiltingSpeed, BlueprintGetter = GetTiltingSpeed,
				Category = "Helicopter Movement", meta = (ClampMin = 0))
	float TiltingSpeed = 5;

	// void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	
	

};


