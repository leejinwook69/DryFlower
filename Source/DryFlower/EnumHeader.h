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
	Corrider		UMETA(DisplayName = "Corrider"),
	PlayerSpawn		UMETA(DisplayName = "PlayerSpawn"),
	EnemySpawn		UMETA(DisplayName = "EnemySpawn"),
	Restricted01	UMETA(DisplayName = "Restricted01"),
	Restricted02	UMETA(DisplayName = "Restricted02"),
	Restricted03	UMETA(DisplayName = "Restricted03"),
	Restricted04	UMETA(DisplayName = "Restricted04"),
	Restricted05	UMETA(DisplayName = "Restricted05"),
	Escape			UMETA(DisplayName = "Escape"),
	Office			UMETA(DisplayName = "Office"),
	Labatory		UMETA(DisplayName = "Labatory"),
	Bathroom		UMETA(DisplayName = "Bathroom"),
};

UENUM()
enum class Arrow : uint8
{
	None,
	Top,
	Bottom,
	Left,
	Right,
};

USTRUCT(Atomic, BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int roomNumber;

	int maxDoorCount;

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