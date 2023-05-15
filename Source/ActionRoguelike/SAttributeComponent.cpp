
// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values
USAttributeComponent::USAttributeComponent()
{
	Health = 100.f;
}

bool USAttributeComponent::IsAlive() const // read only access
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	if (!Delta) return false;
	Health += Delta;
	//OnHealthChanged.Brodcast(nullptr, this, Health, Delta);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

