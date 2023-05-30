// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_HealthSelf.h"
#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealthSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
    if (MyPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
    if (ensure(AttributeComp))
    {
        AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
    }

    return EBTNodeResult::Succeeded;
}