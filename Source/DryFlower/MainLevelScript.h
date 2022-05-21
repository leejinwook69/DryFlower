// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EnumHeader.h"
#include "MainLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class DRYFLOWER_API AMainLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> corriderRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> playerSpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> enemySpawnRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom00;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> dangerRoom04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> escapeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> officeRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> labatoryRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TSubclassOf<AActor> bathRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomActorsRef)
	TArray<FRoomInfo> roomInfo;

	UFUNCTION(BlueprintCallable)
	void InitialRoomSetup();

	//For Algorithm

	//맵에서 생성할 플레이어 스폰 룸 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm)
	int maxPlayerSpawn;

	int currentPlayerSpawn = 0;
	TArray<FRoomInfo*> checkedList;

	//플레이어 스폰룸이 바깥 가장자리로부터 몇 칸까지 생성 허용할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm)
	int minBorder;

	//minBorder값으로 지정된 갯수의 방을 만들지 못했을 경우 여기까지 허용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Algorithm)
	int maxBorder;

	int GetRoomNumberFromIndex(FIntPoint index);
	void RoomCreateAlgorithm(FRoomInfo *currentRoom, FRoomInfo *priviousRoom);
	void AddAnotherPlayerRoom();
	bool CheckTopRoomIs(int roomNum, RoomType type);
	bool CheckBottomRoomIs(int roomNum, RoomType type);
	bool CheckLeftRoomIs(int roomNum, RoomType type);
	bool CheckRightRoomIs(int roomNum, RoomType type);
	bool CheckIsInBound(int roomX, int roomY);
	bool CanMakePlayerSpawnRoom(int targetRoomNum, int distance);
};