// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckHealth.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
    LowHealthFraction = 0.3f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds );

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ensure(AIPawn))
    {
        USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
        if (ensure(AttributeComp))
        {
            bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) < LowHealthFraction;

            UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
            BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
        }
    }
}