// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelScript.h"

void AMainLevelScript::InitialRoomSetup()
{
    //초기화
    roomInfo.Init(FRoomInfo(), 81);
    FRoomInfo *selectRoom = nullptr;
    int count = 0;
    //방번호 부여. 초기값 설정 필요하면 이곳에서
    for(auto& elem : roomInfo)
    {
        elem.roomNumber = count;
        count++;
    }
    //-------------------------------------------------
    //EscapeRoom 우선 선정. 4개의 모서리 중 중복되지 않게 2개 선택
    //처음 생성된 EscapeRoom을 스타팅 룸 인덱스로 선정
    FIntPoint escapeRoomIndex[4] = {FIntPoint(0, 0), FIntPoint(8, 0), FIntPoint(0, 8), FIntPoint(8, 8)};
    FIntPoint startRoomNum;
    int rand = 99;
    count = 0;

    while(count < 2)
    {
        int tempRand = FMath::RandRange(0, 3);
        if(rand != tempRand)
        {
            rand = tempRand;

            if(count == 0)
                startRoomNum = escapeRoomIndex[rand];

            selectRoom = &roomInfo[GetRoomNumberFromIndex(escapeRoomIndex[rand])];
            selectRoom->roomType = RoomType::escape;
            count++;
        }
    }
    //-------------------------------------------------
    //EnemySpawnRoom 선정 (4, 4) 위치 고정
    selectRoom = &roomInfo[GetRoomNumberFromIndex(FIntPoint(4, 4))];
    selectRoom->roomType = RoomType::enemySpawn;
    selectRoom->SetWallAndDoor(true, true, true, true, true, true, true, true);
    
    //1. 현재 위치에서 상/하/좌/우 중 랜덤 1개를 선택하여 그곳이 None이면 그 방향으로 문 생성, 전진
    //2. 리스트에 있는 다른 방들과 비교해서 XY거리가 3 이상인지 체크
    //2-T. 해당 방을 PlayerSpawn으로 만들고 리스트에 추가한 뒤 이전 방 방향으로 문 생성, 탈출
    //2-F. 해당 방을 Corrider로 만들고 다른 방 방향으로 문 생성, 전진
    //3. PlayerSpawn이 12이거나 더이상 진행할 수 없으면 스탑
    //4. PlayerSpawn이 12가 아닌데 더 진행할 수 없는 상황이면 내부 1칸까지도 돌아다니면서 PlayerSpawn 생성해야함

    //UE_LOG(LogTemp, Log, TEXT("start room index : (%d, %d)"), startRoomNum.X, startRoomNum.Y);
}

int AMainLevelScript::GetRoomNumberFromIndex(FIntPoint index)
{
    return (index.Y * 9) + index.X;
}
/*
void AMainLevelScript::SpawnRoom(RoomType roomType, FVector3 spawnLocation, )
{
    FTransform SpawnLocation;
    ABasicRoom *instance = GetWorld()->SpawnActorDeferred<ABasicRoom>(basicRoom, SpawnLocation); //스폰 시작
    instance->InitialStart(i * 9 + j, true, true, true, true, true, true, true, true); //초기화, 변수할당 등
    UGameplayStatics::FinishSpawningActor(instance, instance->GetTransform()); //스폰 종료
}
*/