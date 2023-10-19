// Fill out your copyright notice in the Description page of Project Settings.


#include "SCopter.h"
#include "GameFramework/SpringArmComponent.h"
// #include "Components/SceneComponent.h"
#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"

// Sets default values
ASCopter::ASCopter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    SetRootComponent(Mesh);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepWorldTransform);
	
	ThrusterComp = CreateDefaultSubobject<UPhysicsThrusterComponent>("ThrusterComp");
	ThrusterComp->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(ThrusterComp);
	Arrow->SetArrowColor(FColor::Blue);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(1);
	Mesh->SetAngularDamping(1);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ThrusterComp->SetRelativeRotation(FRotator(-90, 0, 0));
	ThrusterComp->bAutoActivate = false;

	SpringArmComp->TargetArmLength = 800;
	SpringArmComp->SetRelativeRotation((FRotator(-20, 90, 0)));
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritRoll = false;
}

// Called when the game starts or when spawned
void ASCopter::BeginPlay()
{
	Super::BeginPlay();
	
	FVector ThrusterCenterOfMassDifference = ThrusterComp->GetComponentLocation() - Mesh->GetCenterOfMass();
	FRotator InvertedMeshRotation = Mesh->GetComponentRotation().GetInverse();
	FVector CenterOfMassActualOffset = InvertedMeshRotation.RotateVector(ThrusterCenterOfMassDifference);
	FVector CenterOfMassWantedOffset = FVector(CenterOfMassActualOffset.X, CenterOfMassActualOffset.Y, 0);
	Mesh->SetCenterOfMass(CenterOfMassWantedOffset, FName());
	
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ASCopter::OnActorOverlap);
}

void ASCopter::MouseRight(float Value)
{
	SpringArmComp->AddRelativeRotation(FRotator(0, Value, 0));
}

void ASCopter::MouseUp(float Value)
{
	SpringArmComp->AddRelativeRotation(FRotator(Value, 0, 0));
}

void ASCopter::MoveUp(float Value)
{
	float DesiredUpForce = Value * VarableUpForce + ConstantUpForce;
	ThrusterComp->ThrustStrength = DesiredUpForce / GetActorUpVector().Z * Mesh->GetMass();
}

void ASCopter::RotateRight(float Value)
{
	Mesh->AddTorqueInDegrees(GetActorUpVector() * Value * YawRoatationSpeed,  FName(), true);
}

void ASCopter::TiltForward(float Value)
{
	float DesiredAngle = Value * DesiredTiltAngle + Mesh->GetComponentRotation().Pitch;
	float ClampedVAlue = FMath::Clamp(DesiredAngle, -TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorRightVector() * ClampedVAlue * TiltingSpeed, FName(), true);
}

void ASCopter::TiltRight(float Value)
{
	float DesiredAngle = Value * -30 + Mesh->GetComponentRotation().Roll;
	float ClampledValue = FMath::Clamp(DesiredAngle, TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorForwardVector() * ClampledValue * TiltSpeedClampBound, FName(), true);
}

void ASCopter::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASCharacter* Char = Cast<ASCharacter>(OtherActor))
	{
		APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		PC->Possess(this);
	}
}

// Called every frame
void ASCopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Vel = ThrusterComp->GetComponentVelocity();
	FString DebugMsg = Vel.ToString();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White,  FString::Printf(TEXT("Velocity: %s"), *DebugMsg));
}

// Called to bind functionality to input
void ASCopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MouseRight", this, &ASCopter::MouseRight);
	PlayerInputComponent->BindAxis("MouseUp", this,  &ASCopter::MouseUp);
	PlayerInputComponent->BindAxis("MoveUp", this,  &ASCopter::MoveUp);
	PlayerInputComponent->BindAxis("RotateRight", this,  &ASCopter::RotateRight);
	PlayerInputComponent->BindAxis("TiltForward", this,  &ASCopter::TiltForward);
	PlayerInputComponent->BindAxis("TiltRight", this, &ASCopter::TiltRight);
}

