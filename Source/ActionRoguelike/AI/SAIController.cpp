// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behaviour Tree is Nullptr! Please assign behaviour tree in your AI controller")))
	{
		RunBehaviorTree(BehaviorTree); // Only server runs this logic
	}

	//staring code:
	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0); // player index > 0 would normally be used in case of a split screen
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}
