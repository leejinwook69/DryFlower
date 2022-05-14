// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicRoom.h"

// Sets default values
ABasicRoom::ABasicRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

