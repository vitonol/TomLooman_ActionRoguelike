
// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values
ASAttributeComponent::ASAttributeComponent()
{
	Health = 100.f;
}


bool ASAttributeComponent::ApplyHealthChange(float Delta)
{
	if (!Delta) return false;
	Health += Delta;
	return true;
}

