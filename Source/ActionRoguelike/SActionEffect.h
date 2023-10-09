#pragma once
#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()
public:
	void StartAction_Implementation(AActor* Instigaor) override;

	void StopAction_Implementation(AActor* Instigaor) override;
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Effect")
	float Duration;

	// Time between ticks to apply effect
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Effect")
	float Period;

	FTimerHandle PeriodTimerHandle;
	
	FTimerHandle DurationTimerHandle;

	UFUNCTION(BlueprintNativeEvent, Category="Effecft")
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	USActionEffect();
	
};
