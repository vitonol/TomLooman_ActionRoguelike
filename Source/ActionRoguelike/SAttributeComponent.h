// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAttributeComponent.generated.h"

/*
	instead of checking every frame that code has changed, we can just "subscribe" to a change get notified.
	efficient and conviniet
 */

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS()
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	USAttributeComponent();

protected:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;

	/*
	 * NetMulticast is good for transient events like exolosion
	 *
	 * Rep/WNotify is good for state of the things, also will work for later joiners
	 *
	 *NetMulticast, Reliable ignores relevancy
	 */
	
	UFUNCTION(NetMulticast, Reliable) //@TODO Fixme:  should be Unreliable because it is cosmetic
	void MulticastHealthChanged(AActor* Instigator,  float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable) 
	void MulticastRageChanged(AActor* Instigator,  float NewRage, float Delta);
	
public:

	UFUNCTION(Exec)
	bool Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UPROPERTY(BlueprintAssignable,  Category = "Attributes") // lets us in UI to bind or subscribe
 	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable,  Category = "Attributes")
	FOnAttributeChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);
};
