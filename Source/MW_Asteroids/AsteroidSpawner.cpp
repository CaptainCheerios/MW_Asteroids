// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"
// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}


void AAsteroidSpawner::Initialize()
{
	int index = 0; 
	//Create the array of indices here, so we don't keep recreating it and we can just keep shuffling it later
	//Make if the developer missed adding a spawn area to the array we don't want to add its index, and throw an error to let them know they missed it.
	for (ATriggerVolume* triggerVolume : spawnAreas)
	{
		if (triggerVolume != nullptr)
		{
			indices.Add(index);
		}
		else
			UE_LOG(LogTemp, Error, TEXT("ERROR You have not added all the spawn volumes"));
		index++;
	}
}

int AAsteroidSpawner::FindBestSpawnArea_Implementation()
{	
	if (indices.Num() > 0)
	{
		TArray<FAsteroidCountStruct> areaResults;	
		RandomizeArray(indices);
		for (int i : indices)
		{
			if (!CheckForPlayer(i))
			{
				areaResults.Add(FAsteroidCountStruct(i, GetAsteroidCount(i)));
				if (areaResults.Last().m_asteroidCount == 0)
					return i;
			}
		}
		FAsteroidCountStruct* bestResult = &areaResults[0];
		for (FAsteroidCountStruct currentSpawn : areaResults)
		{
				if (bestResult->m_asteroidCount > currentSpawn.m_asteroidCount)
					bestResult = &currentSpawn;
		}
		return bestResult->m_index;
	}
	return -1;
}


///Randomize the array, randomly swap indices aroudn
void AAsteroidSpawner::RandomizeArray(TArray<int>& originalArray)
{
	int originalArrayLength = originalArray.Num();
	for (int i = 0; i < originalArrayLength; i++)
	{
		int j = FMath::RandRange(0, originalArray.Num() - 1);
		originalArray.Swap(i, j);
	}
}

//Check if we have a player in the specified spawn area index.
bool AAsteroidSpawner::CheckForPlayer(int spawnAreaIndex)   
{
	TSet<AActor*> result;
	spawnAreas[spawnAreaIndex]->GetOverlappingActors(result, APlayerControlledShip::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("SpawnArea: %d || PlayerShips: %d"), spawnAreaIndex, result.Num());
	return (result.Num() > 0);
}

//Check how many asteroids are in the trigger volume at the index
int AAsteroidSpawner::GetAsteroidCount(int spawnAreaIndex)
{
	TSet<AActor*> result;
	spawnAreas[spawnAreaIndex]->GetOverlappingActors(result, AAsteroid::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("SpawnArea: %d || Asteroids: %d"), spawnAreaIndex, result.Num());
	return result.Num();
}

void AAsteroidSpawner::SpawnAsteroid(TSubclassOf<class AAsteroid> asteroidClass)
{
	int spawnIndex = FindBestSpawnArea();
	//if we get -1 we should just exit here because we did not get a spawn point
	if (spawnIndex < 0)
		return;

	//Spawn the asteroid
 	AAsteroid* asteroid = GetWorld()->SpawnActor<AAsteroid>(asteroidClass, 
											GetSpawnPosition(*spawnAreas[spawnIndex]),	// Get the spawn position
								FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));	// Create a random rotation
	asteroid->AddRandomVelocity();
}

FVector AAsteroidSpawner::GetSpawnPosition(const ATriggerVolume& triggerVolume)
{
	FVector spawnLocation = triggerVolume.GetActorLocation();
	FVector spawnExtents = triggerVolume.GetBounds().BoxExtent;
	spawnLocation.X += FMath::FRandRange(borderBoundary - spawnExtents.X, spawnExtents.X - borderBoundary);
	spawnLocation.Y += FMath::FRandRange(borderBoundary - spawnExtents.Y, spawnExtents.Y - borderBoundary);
	spawnLocation.Z = spawnHeight;
	return spawnLocation;
}

