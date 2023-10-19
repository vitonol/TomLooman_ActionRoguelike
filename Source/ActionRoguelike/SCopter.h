// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsThruster.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "SCopter.generated.h"

class UStaticMeshComponent;
class UPhysicsThrusterComponent;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;

// class APhysicsThruster;;

UCLASS()
class ACTIONROGUELIKE_API ASCopter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASCopter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPhysicsThrusterComponent* ThrusterComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	void MouseRight(float Value);

	void MouseUp(float Value);
	
	void MoveUp(float Value);

	void RotateRight(float Value);
	
	void TiltForward(float Value);

	void TiltRight(float Value);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float VarableUpForce = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float ConstantUpForce = 970;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float YawRoatationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float DesiredTiltAngle = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float TiltSpeedClampBound = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter Movement")
	float TiltingSpeed = 5;

	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};


