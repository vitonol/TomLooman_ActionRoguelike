// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, USActionComponent*, OwningComp, USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTag;
	
	UFUNCTION(BlueprintCallable, Category="Acftion")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Acftion")
	void RemoveAction(USAction* ActionToRemove);

	/* Returns first occurrence of action matching the class provided */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category="Acftion")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Acftion")
	bool StopActionByName(AActor* Instigator, FName ActionName);	
	
	USActionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);
	
	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);
	
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Replicated)   // whenever we are dealing with poiters or memery Uproperty is useful to let UE know that there some memomery to keep in mind  
	TArray<USAction*> Actions;

public:

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
