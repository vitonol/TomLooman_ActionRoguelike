// Fill out your copyright notice in the Description page of Project Settings.


  #include "SAction.h"

#include "ActionRoguelike.h"
#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	// ensureAlways(bIsRunning); // No Longer  needed bc On_Rep
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTag.RemoveTags(GrantsTags);
	
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp=NewActionComp;
}

UWorld* USAction::GetWorld() const
{
	//this outer has been set thru a NewObj<T>
	if (AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	// AActor* Actor = Cast<AActor>(GetOuter());
	// return Actor->GetComponentByClass(USActionComponent::StaticClass());
	
	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) return false;
	
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTag.HasAny(BlockedTags))
	{
		false;
	}
	return true;
}

void USAction::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}