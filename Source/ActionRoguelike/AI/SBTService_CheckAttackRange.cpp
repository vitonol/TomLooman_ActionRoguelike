

#include "SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Check distance between AI pawn and the target actor

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent(); // using . instead of -> because we are passing it by reference
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{ 
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn)) // if this pawn is null and we still running behaviour tree then we probably did something wrong, because as soon as AICharacter is dead or gone we should be stopping behaviour tree
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					
					bool bWithinRange = DistanceTo < MaxAIDistatnce;

					bool bHasLOF = false;
					if (bWithinRange) bHasLOF = MyController->LineOfSightTo(TargetActor);

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOF));
				}
			}
		}
	}
	
}
