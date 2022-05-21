// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelScript.h"

void AMainLevelScript::InitialRoomSetup()
{
    //초기화
    roomInfo.Init(FRoomInfo(), 81);
    FRoomInfo *selectRoom = nullptr;
    int roomNum = 99;
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
    FRoomInfo *startRoom;
    int rand = 99;
    count = 0;

    while(count < 2)
    {
        int tempRand = FMath::RandRange(0, 3);
        if(rand != tempRand)
        {
            rand = tempRand;
            roomNum = GetRoomNumberFromIndex(escapeRoomIndex[rand]);

            if(count == 0)
                startRoom = &roomInfo[roomNum];

            selectRoom = &roomInfo[roomNum];
            selectRoom->roomType = RoomType::escape;
            selectRoom->maxDoorCount = 1;
            checkedList.Add(selectRoom);
            count++;
        }
    }
    //-------------------------------------------------
    //EnemySpawnRoom 선정 (4, 4) 위치 고정
    roomNum = GetRoomNumberFromIndex(FIntPoint(4, 4));
    selectRoom = &roomInfo[roomNum];
    selectRoom->roomType = RoomType::enemySpawn;
    selectRoom->maxDoorCount = 4;
    selectRoom->SetWallAndDoor(true, true, true, true, true, true, true, true);
    checkedList.Add(selectRoom);
    //-------------------------------------------------
    //알고리즘 시작
    RoomCreateAlgorithm(startRoom, nullptr);
    for(auto& elem : roomInfo)
    {
        //UE_LOG(LogTemp, Log, TEXT("[%d, %d]maxDoorLeft : %d"), elem.roomNumber % 9, elem.roomNumber / 9, elem.maxDoorCount);
    }
    //1. 현재 위치에서 상/하/좌/우 중 랜덤 1개를 선택하여 그곳이 None이면 그 방향으로 문 생성, 전진
    //2. 리스트에 있는 다른 방들과 비교해서 XY거리가 3 이상인지 체크
    //2-T. 해당 방을 PlayerSpawn으로 만들고 리스트에 추가한 뒤 이전 방 방향으로 문 생성, 탈출
    //2-F. 해당 방을 Corrider로 만들고 다른 방 방향으로 문 생성, 전진
    //3. PlayerSpawn이 12이거나 더이상 진행할 수 없으면 스탑
    //4. PlayerSpawn이 12가 아닌데 더 진행할 수 없는 상황이면 내부 1칸까지도 돌아다니면서 PlayerSpawn 생성해야함

    //UE_LOG(LogTemp, Log, TEXT("start room index : (%d, %d)"), startRoomNum.X, startRoomNum.Y);
}

void AMainLevelScript::RoomCreateAlgorithm(FRoomInfo *currentRoom, FRoomInfo *priviousRoom)
{
    int currentRoomNum = currentRoom->roomNumber;
    
    if(currentRoomNum < 0 || currentRoomNum > 81)
        return;
    
    TArray<int> visited;
    while(currentRoom->maxDoorCount > 0 || visited.Num() == 4 || currentPlayerSpawn < maxPlayerSpawn)
    {
        int rand = FMath::RandRange(0, 3);
        if(visited.Contains(rand))
            continue;
        else
        {
            visited.Add(rand);
            int currentRoomX = currentRoomNum % 9;
            int currentRoomY = currentRoomNum / 9;
            switch(rand)
            {
                case 0:
                    if(CheckIsInBound(currentRoomX, currentRoomY - 1))
                    {
                        if(CheckTopRoomIsNone(currentRoomNum))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum - 9];

                            currentRoom->topWall = true;
                            currentRoom->topDoor = true;
                            nextRoom->bottomWall = true;
                            nextRoom->bottomDoor = true;

                            if(CanMakePlayerSpawnRoom(nextRoom->roomNumber, 3))
                            {
                                currentPlayerSpawn++;
                                currentRoom->maxDoorCount--;
                                nextRoom->maxDoorCount = 0;
                                nextRoom->roomType = RoomType::playerSpawn;
                                checkedList.Add(nextRoom);
                            }
                            else
                            {
                                nextRoom->roomType = RoomType::corrider;
                                nextRoom->maxDoorCount--;
                                currentRoom->maxDoorCount--;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 1:
                    if(CheckIsInBound(currentRoomX, currentRoomY + 1))
                    {
                        if(CheckBottomRoomIsNone(currentRoomNum))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum + 9];

                            currentRoom->bottomWall = true;
                            currentRoom->bottomDoor = true;
                            nextRoom->topWall = true;
                            nextRoom->topDoor = true;

                            if(CanMakePlayerSpawnRoom(nextRoom->roomNumber, 3))
                            {
                                currentPlayerSpawn++;
                                currentRoom->maxDoorCount--;
                                nextRoom->maxDoorCount = 0;
                                nextRoom->roomType = RoomType::playerSpawn;
                                checkedList.Add(nextRoom);
                            }
                            else
                            {
                                nextRoom->roomType = RoomType::corrider;
                                nextRoom->maxDoorCount--;
                                currentRoom->maxDoorCount--;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 2:
                    if(CheckIsInBound(currentRoomX - 1, currentRoomY))
                    {
                        if(CheckLeftRoomIsNone(currentRoomNum))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum - 1];

                            currentRoom->leftWall = true;
                            currentRoom->leftDoor = true;
                            nextRoom->rightWall = true;
                            nextRoom->rightDoor = true;

                            if(CanMakePlayerSpawnRoom(nextRoom->roomNumber, 3))
                            {
                                currentPlayerSpawn++;
                                currentRoom->maxDoorCount--;
                                nextRoom->maxDoorCount = 0;
                                nextRoom->roomType = RoomType::playerSpawn;
                                checkedList.Add(nextRoom);
                            }
                            else
                            {
                                nextRoom->roomType = RoomType::corrider;
                                nextRoom->maxDoorCount--;
                                currentRoom->maxDoorCount--;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 3:
                    if(CheckIsInBound(currentRoomX + 1, currentRoomY))
                    {
                        if(CheckRightRoomIsNone(currentRoomNum))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum + 1];

                            currentRoom->rightWall = true;
                            currentRoom->rightDoor = true;
                            nextRoom->leftWall = true;
                            nextRoom->leftDoor = true;

                            if(CanMakePlayerSpawnRoom(nextRoom->roomNumber, 3))
                            {
                                currentPlayerSpawn++;
                                currentRoom->maxDoorCount--;
                                nextRoom->maxDoorCount = 0;
                                nextRoom->roomType = RoomType::playerSpawn;
                                checkedList.Add(nextRoom);
                            }
                            else
                            {
                                nextRoom->roomType = RoomType::corrider;
                                nextRoom->maxDoorCount--;
                                currentRoom->maxDoorCount--;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                default:
                break;
            }
        }
    }
    return;
}

bool AMainLevelScript::CanMakePlayerSpawnRoom(int targetRoomNum, int distance)
{
    int targetX = targetRoomNum % 9;
    int targetY = targetRoomNum / 9;

    for(auto& elem : checkedList)
    {
        int checkX = elem->roomNumber % 9;
        int checkY = elem->roomNumber / 9;

        if(FMath::Abs(targetX - checkX) + FMath::Abs(targetY - checkY) < distance)
            return false;
    }

    return true;
}

int AMainLevelScript::GetRoomNumberFromIndex(FIntPoint index)
{
    return index.Y * 9 + index.X;
}

bool AMainLevelScript::CheckTopRoomIsNone(int roomNum)
{
    if(roomNum / 9 == 0)
        return false;

    if(roomInfo[roomNum - 9].roomType == RoomType::None)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckBottomRoomIsNone(int roomNum)
{
    if(roomNum / 9 == 8)
        return false;

    if(roomInfo[roomNum + 9].roomType == RoomType::None)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckLeftRoomIsNone(int roomNum)
{
    if(roomNum % 9 == 0)
        return false;

    if(roomInfo[roomNum - 1].roomType == RoomType::None)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckRightRoomIsNone(int roomNum)
{
    if(roomNum % 9 == 8)
        return false;

    if(roomInfo[roomNum + 1].roomType == RoomType::None)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckIsInBound(int roomX, int roomY)
{
    if(roomX < 0 || roomX > 8 || roomY < 0 || roomY > 8)
        return false;
    else if(roomX >= minBorder && roomX <= 8 - minBorder && roomY >= minBorder && roomY <= 8 - minBorder)
        return false;
    else
        return true;
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