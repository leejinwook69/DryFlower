// Copyright Epic Games, Inc. All Rights Reserved.


#include "DryFlowerGameModeBase.h"
//#include "BasicRoom.h" //스폰할 블루프린트 참조
#include "Kismet/GameplayStatics.h" //FinishSpawningActor를 사용하기 위해 추가

void ADryFlowerGameModeBase::Start() //블루프린트 BeginPlay()에서 호출
{
    /*
    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Start!!")); //이 구문이 실행되는지 테스트용 출력

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            FTransform SpawnLocation;
            ABasicRoom *instance = GetWorld()->SpawnActorDeferred<ABasicRoom>(basicRoom, SpawnLocation); //스폰 시작
            instance->InitialStart(i * 9 + j, true, true, true, true, true, true, true, true); //초기화, 변수할당 등
            roomList.Add(instance); //작동하지 않음
            UGameplayStatics::FinishSpawningActor(instance, instance->GetTransform()); //스폰 종료
        }
    }
    */
}

void ADryFlowerGameModeBase::MakePlayerSpawnRoom()
{
    /*
    RoomType rooms[9][9];
    int startX = 0;
    int startY = 0;
    while(true)
    {
        startX = FMath::RandRange(0, 8);
        startY = FMath::RandRange(0, 8);
        if(CanMakePlayerSpawnRoom(startX, startY))
            break;
    }
    */
}

bool ADryFlowerGameModeBase::CanMakePlayerSpawnRoom(int x, int y)
{
    /*
    if(x > 1 && x < 7 && y > 1 && y < 7)
        return false;
    else if(x < 0 || x > 8 || y < 0 || y > 8)
        return false;
    else
        return true;
    */

   return true;
}