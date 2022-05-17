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

void ABasicRoom::InitialStart(bool leftWallExist, bool rightWallExist, bool topWallExist, bool bottomWallExist, 
						bool leftDoorExist, bool rightDoorExist, bool topDoorExist, bool bottomDoorExist)
{
	this->leftWall = leftWallExist;
	this->rightWall = rightWallExist;
	this->topWall = topWallExist;
	this->bottomWall = bottomWallExist;
	this->leftDoor = leftDoorExist;
	this->rightDoor = rightDoorExist;
	this->topDoor = topDoorExist;
	this->bottomDoor = bottomDoorExist;
}

