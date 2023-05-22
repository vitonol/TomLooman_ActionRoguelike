// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EngineUtils.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true );
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) // beter version of GetActorsOfClass, only returns instances of that class
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot); // uses static funncftion
		{
			if (ensure(AttributeComp) && AttributeComp->IsAlive())
			{
				NrOfAliveBots++; 
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NrOfAliveBots);
	
	float MaxBotCount = 10.f;

	if(DifficultyCurve) MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At max capacity. Skipping Bots spawn"), NrOfAliveBots);
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance)) QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted); // generally we don't wrap inside function, but 
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	// gives result location where to spawn bots
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0 )
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0],FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.f, 20, FColor::Purple, false, 60.f);
	}
}
