// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "Asteroid.h"
#include "PlayerControlledShip.h"
#include "AsteroidSpawner.generated.h"


USTRUCT()
struct FAsteroidCountStruct
{
	GENERATED_BODY();

	int m_index;
	int m_asteroidCount;

	FAsteroidCountStruct() {};
	FAsteroidCountStruct(int index, int asteroidCount) : m_index(index), m_asteroidCount(asteroidCount){};
	
	UPROPERTY()
	UObject* SafeObjectPointer;
};


UCLASS()
class MW_ASTEROIDS_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CheckForPlayer(int spawnAreaIndex);
	int GetAsteroidCount(int spawnAreaIndex);

	UFUNCTION(BlueprintNativeEvent)
	int FindBestSpawnArea();
	int FindBestSpawnArea_Implementation();

	FVector GetSpawnPosition(const ATriggerVolume& triggerVolume);
	
	
public:	
	UPROPERTY(EditAnywhere,Category="AsteroidSpawner")
	TArray<ATriggerVolume*> spawnAreas;	

	UFUNCTION(BlueprintCallable, Category="Asteroids|Spawn", DisplayName = "Spawn New Asteroid")
	void SpawnAsteroid(TSubclassOf<class AAsteroid> asteroidClass);

	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0"))
	float borderBoundary = 30.0f;

	UPROPERTY(EditAnywhere)
	float spawnHeight = 30.0f;

private:
	TArray<int> RandomizeArray(TArray<int> originalArray);
};
