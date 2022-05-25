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
	
	SetActorLocation(FVector(roomNumber % 9 * 1500, roomNumber / 9 * 900, 0), false, 0, ETeleportType::None);
}

// Called every frame
void ABasicRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicRoom::InitialStart(int roomNum, RoomType roomTypeData, bool leftWallExist, bool rightWallExist, bool topWallExist, bool bottomWallExist, 
						bool leftDoorExist, bool rightDoorExist, bool topDoorExist, bool bottomDoorExist)
{
	this->roomNumber = roomNum;
	this->roomType = roomTypeData;
	this->leftWall = leftWallExist;
	this->rightWall = rightWallExist;
	this->topWall = topWallExist;
	this->bottomWall = bottomWallExist;
	this->leftDoor = leftDoorExist;
	this->rightDoor = rightDoorExist;
	this->topDoor = topDoorExist;
	this->bottomDoor = bottomDoorExist;

}

//府敲府纳捞记 包访
void ABasicRoom::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABasicRoom, roomNumber);
    DOREPLIFETIME(ABasicRoom, roomType);
    DOREPLIFETIME(ABasicRoom, leftWall);
    DOREPLIFETIME(ABasicRoom, rightWall);
    DOREPLIFETIME(ABasicRoom, topWall);
    DOREPLIFETIME(ABasicRoom, bottomWall);
    DOREPLIFETIME(ABasicRoom, leftDoor);
    DOREPLIFETIME(ABasicRoom, rightDoor);
    DOREPLIFETIME(ABasicRoom, topDoor);
    DOREPLIFETIME(ABasicRoom, bottomDoor);

}