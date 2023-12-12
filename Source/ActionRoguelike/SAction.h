// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable) // without Blueprintable we can not make child classses as SAction
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UTexture2D* Icon;

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	// bool bIsRunning;
	FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();
	
public:
	
	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);
	
	/* // FText only for front facing (because it can be localzed, Fstring is mostly for debugging
	 * FName is hashed and allows for fast comparison.
	 * */
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;
	
	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
