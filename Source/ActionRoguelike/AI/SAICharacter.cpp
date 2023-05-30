// Fill out your copyright notice in the Description page of Project Settings.


#include "SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "ActionRoguelike/SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";	
}



void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp-> OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,	float Delta)
{
	if (Delta < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		if (InstigatorActor != this) SetTargetActor(InstigatorActor);

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		if (NewHealth <= 0.f)
		{	  // stop behaviour tree, ragdool, setLifeSpan
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC) AIC->GetBrainComponent()->StopLogic("Killed");

			GetMesh()->SetAllBodiesSimulatePhysics(true);  // Mesh marked as private, so we  can't directly access it, Mesh in our classes is under protected
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTER", nullptr, FColor::White, 4.f, true);
}
 

