// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Acftion")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Acftion")
	bool StartActinoByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Acftion")
	bool StopActionByName(AActor* Instigator, FName ActionName);	
	
	USActionComponent();

protected:

	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	virtual void BeginPlay() override;

	UPROPERTY()   // whenever we are dealing with poiters or memery Uproperty is useful to let UE know that there some memomery to keep in mind  
	TArray<USAction*> Actions;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
