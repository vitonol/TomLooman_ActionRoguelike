// Fill out your copyright notice in the Description page of Project Settings.


#include "SHelicopterSM.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


ASHelicopterSM::ASHelicopterSM()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	// TailRotorSocket = "tail_rotor";
	// RotorSocket = "main_rotor";

	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>("MeshBody");
	RootComponent = MeshBody;

	MeshRotor = CreateDefaultSubobject<UStaticMeshComponent>("Rotor");
	MeshRotor->SetupAttachment(MeshBody);

	MeshBlade = CreateDefaultSubobject<UStaticMeshComponent>("Blade");
	MeshBlade->SetupAttachment(MeshRotor);

	MeshTailRotor = CreateDefaultSubobject<UStaticMeshComponent>("TailRotor");
	MeshTailRotor->SetupAttachment(MeshBody);
	// FTransform SoketTransform = MeshBody->GetSocketTransform(TailRotorSocket);
	
	MeshTailBlade = CreateDefaultSubobject<UStaticMeshComponent>("TailBlade");
	MeshTailBlade->SetupAttachment(MeshTailRotor);

	Springarm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Springarm->SetupAttachment(MeshBody);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Springarm);

	Springarm->SetRelativeLocation(FVector(0,0,220));
	Camera->SetRelativeRotation(FRotator(-20.f, 0,0));
	Springarm->TargetArmLength = 1200.f;
	Springarm->bInheritPitch = false;
	Springarm->bEnableCameraLag = true;
	Springarm->CameraLagSpeed = 4.f;
	Springarm->bEnableCameraRotationLag = true;
	Springarm->CameraRotationLagSpeed = 5.f;
	Springarm->SocketOffset = FVector(0,0,500.f);
		
	SetReplicates(true);
}

void ASHelicopterSM::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// MeshTailRotor->SetupAttachment(MeshBody, TailRotorSocket);
	//
	// MeshRotor->SetupAttachment(MeshBody, RotorSocket);
}

void ASHelicopterSM::BeginPlay()
{
	Super::BeginPlay();

	UpdatePreviousValues();

	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	
	if (ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem * InputSystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

void ASHelicopterSM::TickUpdate(float Delta)
{
	//Update Blade Rotation Speed to match Target
	CurrentBladeRotationSpeed = FMath::FInterpTo(CurrentBladeRotationSpeed, TargetBladeRotationSpeed, Delta, 3.f);
	
	UpdatePreviousValues();

	//Compute Velocity and Acceleration
	Velocity = (GetActorLocation() - PrevPosition) * Delta;
	Acceleration = (Velocity - PrevPosition) * Delta;

	// Apply Gravity
	Acceleration = Acceleration + FVector(0, 0, -981.f);

	// Apply lift
	Acceleration = Acceleration + (GetActorUpVector() * GetCurrentLift());

	// Simulate Forward Acceleration
	const FVector UpVectorNoZ = FVector(GetActorUpVector().X, GetActorUpVector().Y, 0);
	Acceleration = Acceleration + (UpVectorNoZ * GetCurrentLift() * 0.2f);

	// Move Helicopter
	FVector DeltaLocation = Velocity + Acceleration * Delta;
	AddActorWorldOffset(DeltaLocation, true);
}

void ASHelicopterSM::RotateBlades(float Delta)
{
	float SelectRotSpeed;
	if (CurrentBladeRotationSpeed > BlurBladeSpeedThreshold)
	{
		LoadBlurryBladeMesh();
		// MeshBlade->SetStaticMesh(Mesh_BladeBlur);
		// MeshTailBlade->SetStaticMesh(Mesh_TailBladeBlur);
		SelectRotSpeed = BlurBladeSpeedThreshold;
	}
	else
	{
		LoadNormalBladeMesh();
		// MeshBlade->SetStaticMesh(Mesh_Blade);
		// MeshTailBlade->SetStaticMesh(Mesh_TailBlade);
		SelectRotSpeed = CurrentBladeRotationSpeed;
	}

	MeshRotor->AddRelativeRotation(FRotator(0, (SelectRotSpeed * Delta), 0));
	
	MeshTailRotor->AddLocalRotation(FRotator((SelectRotSpeed * Delta * 2.f),0,0));
}

float ASHelicopterSM::GetTurnSpeed()
{
	float ClampedVal = FMath::GetMappedRangeValueClamped(FVector2f(0, 400.f),
															  FVector2f(0, 1.f),
															  GetCurrentLift());
	return TurnSpeed * ClampedVal;
}

float ASHelicopterSM::GetCurrentLift()
{
	return LiftCurve->GetFloatValue(CurrentBladeRotationSpeed);
}

void ASHelicopterSM::UpdatePreviousValues()
{
	PrevPosition = GetActorLocation();
	PrevVelocity = Velocity;
	PrevAcceleration = Acceleration;
}

void ASHelicopterSM::Climb(const FInputActionInstance& Instance)
{
	float PressTime = Instance.GetTriggeredTime();
	float Value = Instance.GetValue().Get<float>();

	if (Value != 0)
	{
		const float D = UGameplayStatics::GetWorldDeltaSeconds(this);
		SetBladeRotationSpeed(Value, D);

		if (bAutoDown) bAutoDown = false;
	}
	else
	{
		bAutoDown = true;
	}
}

void ASHelicopterSM::MoveRight(const FInputActionInstance& Instance)
{
	float Value = Instance.GetValue().Get<float>();
	float D = UGameplayStatics::GetWorldDeltaSeconds(this);

	float LerpSpeed = FMath::IsNearlyEqual(Value, 0.f, 0.05f) ? 2.f : 3.f;
	
	CurrentTurn = FMath::FInterpTo(CurrentTurn, Value * GetTurnSpeed(), D, LerpSpeed);

	AddActorWorldRotation(FRotator(0, CurrentTurn, 0));
	FRotator ActorRotation =  GetActorRotation();
	FRotator NewRotation = FRotator(ActorRotation.Pitch, ActorRotation.Yaw, (CurrentTurn * 45.f));
	SetActorRotation(NewRotation);
}

void ASHelicopterSM::MoveForward(const FInputActionInstance& Instance)
{
	float Value = Instance.GetValue().Get<float>();
	Value = Value * (-1);
	float D = UGameplayStatics::GetWorldDeltaSeconds(this);

	float LerpSpeed = 5.f; //FMath::IsNearlyEqual(Value, 0.f, 0.05f) ? 2.f : 5.f;

	CurrentPitch = FMath::FInterpTo(CurrentPitch, Value * GetTurnSpeed(),D, LerpSpeed );
	// FMath::InterpEaseOut(CurrentPitch, Value * GetTurnSpeed(), D, LerpSpeed);

	const float TargetPitch = CurrentPitch + GetActorRotation().Pitch;
	if (FMath::IsWithin(TargetPitch, -75.f, 75.f))
	{
		AddActorLocalRotation(FRotator(CurrentPitch, 0, 0 ));
	}

	// Set Pitch back to 0 when controller released
	if (FMath::IsNearlyEqual(Value, 0.f, 0.1f))
	{
		FRotator ActorRotation =  GetActorRotation();
		float ReturnPitch = FMath::InterpEaseInOut((float)ActorRotation.Pitch, 0.f, D, 1.f);
		
			//FMath::FInterpTo(ActorRotation.Pitch, 0, D, LerpSpeed);
		SetActorRotation(FRotator(ReturnPitch, ActorRotation.Yaw, ActorRotation.Roll));
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Value: %f"), TargetPitch ));
}

void ASHelicopterSM::RotateYaw(const FInputActionInstance& Instance)
{
	float Value = Instance.GetValue().Get<float>();
	float D = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	CurrentRotation = FMath::FInterpTo(CurrentRotation, Value * GetTurnSpeed(),D, 3.f );

	AddActorLocalRotation(FRotator(0, CurrentRotation, 0));
}

void ASHelicopterSM::SetBladeRotationSpeed(float Value, float DeltaTime)
{
	float ValueToClamp = (Value * DeltaTime * ThrottleUpSpeed) + TargetBladeRotationSpeed;
	TargetBladeRotationSpeed = FMath::Clamp(ValueToClamp ,0.f, MaxBladeRotationSpeed);
}

void ASHelicopterSM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickUpdate(DeltaTime);

	RotateBlades(DeltaTime);

	if (bAutoDown)
	{
		SetBladeRotationSpeed(-0.2f, DeltaTime);
	}
	
	FString string = "Blade Speed: " + FString::SanitizeFloat(TargetBladeRotationSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, string);
}

void ASHelicopterSM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	Input->BindAction(MoveUp, ETriggerEvent::Triggered, this, &ASHelicopterSM::Climb);
	Input->BindAction(MoveUp, ETriggerEvent::Completed, this, &ASHelicopterSM::Climb);

	Input->BindAction(TiltRight, ETriggerEvent::Triggered, this, &ASHelicopterSM::MoveRight);
	Input->BindAction(TiltRight, ETriggerEvent::Completed, this, &ASHelicopterSM::MoveRight);

	Input->BindAction(TiltForward, ETriggerEvent::Triggered, this, &ASHelicopterSM::MoveForward);
	Input->BindAction(TiltForward, ETriggerEvent::Completed, this, &ASHelicopterSM::MoveForward);
	
	
	Input->BindAction(RotateRight, ETriggerEvent::Triggered, this, &ASHelicopterSM::RotateYaw);
	
	// PlayerInputComponent->BindAxis("MoveUp", this, &ASHelicopterSM::MoveUp);
}

