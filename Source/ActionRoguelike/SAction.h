// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
/**
 * 
 */
UCLASS(Blueprintable) // without Blueprintable we can not make child classses as SAction
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigaor);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigaor);
	
	UPROPERTY(EditDefaultsOnly, Category="Action")
	/* // FText only for front facing (because it can be localzed, Fstring is mostly for debugging
	 * FName is hashed and allows for fast comparison.
	 * */
	FName ActionName;

	UWorld* GetWorld() const override;
	 
};
