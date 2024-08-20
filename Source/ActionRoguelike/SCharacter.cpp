// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InterationComponent");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	TimeToHitParamName = "TimeToHit";
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}


FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::HealSelf(float Amount /* 100 */)
{ 
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);	

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	MID = GetMesh()->CreateDynamicMaterialInstance(0);
}

void ASCharacter::DoStuff()
{

	FGraphEventArray Tasks;
	// instead of cloaning array, we moving it;
	// TArray<FVector> arr;
	// auto WorkerThread = FFunctionGraphTask::CreateAndDispatchWhenReady([arr = MoveTemp(arr)]
	// {
	// 	
	// } );

	for (int32 i = 0; i < 10; ++i )
	{
		// Spawns worker thread
		auto WorkerThread = FFunctionGraphTask::CreateAndDispatchWhenReady([]
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(WorkerthreadTaskThingDoesNotMatter);
			FPlatformProcess::Sleep(5.f);//faking work
			
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, TEXT("Doing things"));
			float var{0.f};
		} );
		
		Tasks.Add(WorkerThread);
	}
	
	auto Handle = FFunctionGraphTask::CreateAndDispatchWhenReady([]
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(SomethingElseNoneWorkerSlakerThread);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Orange, TEXT("Finished"));
	}, {/* the stat I want to use to see in Insights*/}, &Tasks, ENamedThreads::GameThread);
}


void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	
	AddMovementInput(ControlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;
	// X = Forward (Red)
	// Y = Right (green)
	// Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, value);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		FVector4 CurrentColor = MID->K2_GetVectorParameterValue("ColorBottom");
		// MID->SetScalarParameterValue(TimeToHitParamName, GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		// GetMesh()->SetVectorParameterValueOnMaterials("ColorBottom",  CurrentColor + FVector4(0.2f, 0.2f,0.2f, 0.3f));

		const float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}
	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
 		DisableInput(PC);
		SetLifeSpan(5.0f);
	}
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent) InteractionComponent->PrimaryInteract();
}
 