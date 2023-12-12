
// Fill out your copyright notice in the Description page of Project Settings.
#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float>CVarDamageMultiplier(TEXT("su.DamageMultipliuer"), 1.f, TEXT("Enable damage multiplier"), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.f;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100.f;

	SetIsReplicatedByDefault(true); 
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* Instigator, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigator, this, NewHealth, Delta); // Fire and forget, while Health var is still replicated
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* Instigator, float NewRage, float Delta)
{
	
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -GetHealthMax());
}


bool USAttributeComponent::IsAlive() const // read only access
{
	return Health > 0.0f;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged()) return false;

	if (Delta < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	float ActualDelta = Health - OldHealth;

	// Is server ? 
	if (GetOwner()->HasAuthority())
	{
		Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);
		if (ActualDelta != 0.f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		
		// DIED
		if (ActualDelta < 0.f && Health == 0.f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>(); // GM only exists on the server
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	// DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_OwnerOnly); // Optimization, to only send  nessesary data. Also better for CPU:  COND_InitialOnly

	
}