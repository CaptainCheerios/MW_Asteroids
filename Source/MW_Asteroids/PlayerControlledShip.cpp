// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlledShip.h"

// Sets default values
APlayerControlledShip::APlayerControlledShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerControlledShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerControlledShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerControlledShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

