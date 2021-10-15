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
}


int AAsteroidSpawner::FindBestSpawnArea_Implementation()
{	
	if (spawnAreas.Num() > 0)
	{
		TArray<int> indices;
		TArray<FAsteroidCountStruct> areaResults;
		
		for (int index = 0; index < spawnAreas.Num(); index++)
		{
			indices.Add(index);
		}
		indices = RandomizeArray(indices);
		for (int i : indices)
		{
			if (!CheckForPlayer(i))
			{
				areaResults.Add(FAsteroidCountStruct(i, GetAsteroidCount(i)));
				if (areaResults.Last().m_asteroidCount == 0)
					return i;
			}
		}
		FAsteroidCountStruct* bestResult = nullptr;
		for (FAsteroidCountStruct currentSpawn : areaResults)
		{
			if (bestResult != nullptr)
				if (bestResult->m_asteroidCount > currentSpawn.m_asteroidCount)
					bestResult = &currentSpawn;
			else
				bestResult = &currentSpawn;
		}
		return bestResult->m_index;
	}
	return -1;
}
///Randomize the array, the next step would be to not copy and instead to do a move.
TArray<int> AAsteroidSpawner::RandomizeArray(TArray<int> originalArray)
{
	TArray<int> tempArray = originalArray;
	TArray<int> randomizedArray;
	while (tempArray.Num() > 0)
	{
		int index = FMath::RandRange(0, tempArray.Num() - 1);
		randomizedArray.Add(tempArray[index]);
		tempArray.RemoveAt(index);
	}
	return randomizedArray;
}

bool AAsteroidSpawner::CheckForPlayer(int spawnAreaIndex)
{
	TSet<AActor*> result;
	spawnAreas[spawnAreaIndex]->GetOverlappingActors(result, APlayerControlledShip::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("SpawnArea: %d || PlayerShips: %d"), spawnAreaIndex, result.Num());
	return (result.Num() > 0);
}

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

