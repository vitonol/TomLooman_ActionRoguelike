#include "SActionEffect.h"
#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigaor)
{
	Super::StartAction_Implementation(Instigaor);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigaor);

		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigaor);

		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigaor)
{

	// So we don't miss out on a last remaining tick
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigaor);
	}
	
	Super::StopAction_Implementation(Instigaor);

	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

float USActionEffect::GetTimeRemainig() const
{
	if (AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>() )
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	
	return Duration;
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}
