// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumHeader.generated.h"

UCLASS()
class DRYFLOWER_API AEnumHeader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnumHeader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UENUM(BlueprintType)
enum class RoomType : uint8
{
	None			UMETA(DispalyName = "None"),
	corrider		UMETA(DisplayName = "Corrider"),
	playerSpawn		UMETA(DisplayName = "PlayerSpawn"),
	enemySpawn		UMETA(DisplayName = "EnemySpawn"),
	danger			UMETA(DisplayName = "Danger"),
	escape			UMETA(DisplayName = "Escape"),
	office			UMETA(DisplayName = "Office"),
	labatory		UMETA(DisplayName = "Labatory"),
	bath			UMETA(DisplayName = "Bath"),
};

USTRUCT(Atomic, BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int roomNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	RoomType roomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool leftWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool rightWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool topWall;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bottomWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool leftDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool rightDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool topDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bottomDoor;

	void SetWallAndDoor(bool left_wall, bool right_wall, bool top_wall, bool bottom_wall, bool left_door, bool right_door, bool top_door, bool bottom_door)
	{
		leftWall = left_wall;
		rightWall = right_wall;
		topWall = top_wall;
		bottomWall = bottom_wall;

		leftDoor = left_door;
		rightDoor = right_door;
		topDoor = top_door;
		bottomDoor = bottom_door;
	}
};