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

	TailRotorSocket = "tail_rotor";

	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>("MeshBody");
	RootComponent = MeshBody;

	Rotor = CreateDefaultSubobject<UStaticMeshComponent>("Rotor");
	Rotor->SetupAttachment(MeshBody);

	Blade = CreateDefaultSubobject<UStaticMeshComponent>("Blade");
	Blade->SetupAttachment(Rotor);

	TailRotor = CreateDefaultSubobject<UStaticMeshComponent>("TailRotor");
	// FTransform SoketTransform = MeshBody->GetSocketTransform(TailRotorSocket);
	

	TailBlade = CreateDefaultSubobject<UStaticMeshComponent>("TailBlade");
	TailBlade->SetupAttachment(TailRotor);

	Springarm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Springarm->SetupAttachment(MeshBody);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Springarm);
	
	SetReplicates(true);
}

void ASHelicopterSM::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	TailRotor->SetupAttachment(MeshBody, TailRotorSocket);
	
}

void ASHelicopterSM::BeginPlay()
{
	Super::BeginPlay();

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


void ASHelicopterSM::MoveUp(float Value)
{
	float D = UGameplayStatics::GetWorldDeltaSeconds(this);
	float ValueToClamp = Value* (-1.f) * D * ThrottleUpSpeed + TargetBladeRotationSpeed;
	TargetBladeRotationSpeed = FMath::Clamp(ValueToClamp ,0.f, MaxBladeRotationSpeed);
	
	FString mes = FString::SanitizeFloat(TargetBladeRotationSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, mes );
}

void ASHelicopterSM::Move(const FInputActionInstance& Instance)
{
	auto g = Instance.GetValue().Get<float>();
	float PressTime = Instance.GetTriggeredTime();
	FString string = FString::SanitizeFloat(PressTime);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, string);
}

void ASHelicopterSM::Released(const FInputActionInstance& Instance)
{
}

void ASHelicopterSM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASHelicopterSM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	Input->BindAction(MoveUpp, ETriggerEvent::Triggered, this, &ASHelicopterSM::Move);

	Input->BindAction(MoveUpp, ETriggerEvent::Completed, this, &ASHelicopterSM::Released);

	// PlayerInputComponent->BindAxis("MoveUp", this, &ASHelicopterSM::MoveUp);

}

