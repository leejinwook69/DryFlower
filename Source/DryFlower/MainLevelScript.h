// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EnumHeader.h"
#include "MyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MainLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class DRYFLOWER_API AMainLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> corriderRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> playerSpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> enemySpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> dangerRoom00;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> dangerRoom01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> dangerRoom02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> dangerRoom03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> dangerRoom04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> escapeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> officeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> labatoryRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TSubclassOf<AActor> bathRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TArray<FRoomInfo> roomInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef, Replicated)
	TArray<AActor*> pawnList;

	UFUNCTION(BlueprintCallable)
	void InitialRoomSetup();

	//For Algorithm

	//맵에서 생성할 플레이어 스폰 룸 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm, Replicated)
	int maxPlayerSpawn;

	UPROPERTY(Replicated)
	int currentPlayerSpawn = 0;

	//UPROPERTY(Replicated)
	TArray<FRoomInfo*> checkedList;

	//플레이어 스폰룸이 바깥 가장자리로부터 몇 칸까지 생성 허용할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm, Replicated)
	int minBorder;

	//minBorder값으로 지정된 갯수의 방을 만들지 못했을 경우 여기까지 허용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm, Replicated)
	int maxBorder;

	void RoomCreateAlgorithm(int currentRoomNum);
	void AddAnotherPlayerRoom(int distance);
	bool CheckIsInBound(int roomIndex, Arrow arrow, int minX, int maxX, int minY, int maxY, int width);
	bool CanMakePlayerSpawnRoom(int currentRoomNum, Arrow arrow, int distance);
	int GetRoomNum(int currentRoomNum, Arrow arrow);
	RoomType GetRoomType(int currentRoomNum, Arrow arrow);
	void SetRoomType(int currentRoomNum, Arrow arrow, RoomType roomType);
	void SetRoomDoor(int currentRoomNum, Arrow arrow, bool active);
	void SetRoomWall(int currentRoomNum, Arrow arrow, bool active);
	bool CheckWallExist(int currentRoomNum, Arrow arrow);
	void ConnectingTest();
	int GetDestinationDistance(int currentRoomNum, int destinationRoomNum, int beforeRoomNum);
	void MakeRestrictedRoom();
};