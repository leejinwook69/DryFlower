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
    UE_LOG(LogTemp, Log, TEXT("CurrentPlayerSpawnRoom : %d"), currentPlayerSpawn);
    for(auto& elem : checkedList)
    {
        if(elem->roomType == RoomType::playerSpawn)
            UE_LOG(LogTemp, Log, TEXT("PlayerSpawnRoom index : [%d, %d]"), elem->roomNumber % 9, elem->roomNumber / 9);
    }
    AddAnotherPlayerRoom();
    UE_LOG(LogTemp, Log, TEXT("CurrentPlayerSpawnRoom : %d"), currentPlayerSpawn);
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
    while(currentPlayerSpawn < maxPlayerSpawn)
    {
        if(currentRoom->maxDoorCount <= 0)
            return;
        if(visited.Num() >= 4)
            return;

        int rand = FMath::RandRange(0, 3);
        if(visited.Contains(rand))
        {
            continue;
        }
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
                        if(CheckTopRoomIs(currentRoomNum, RoomType::None))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum - 9];

                            currentRoom->topDoor = true;
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
                                nextRoom->bottomWall = false;
                                currentRoom->maxDoorCount--;
                                currentRoom->topWall = false;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 1:
                    if(CheckIsInBound(currentRoomX, currentRoomY + 1))
                    {
                        if(CheckBottomRoomIs(currentRoomNum, RoomType::None))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum + 9];

                            currentRoom->bottomDoor = true;
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
                                nextRoom->topWall = false;
                                currentRoom->maxDoorCount--;
                                currentRoom->bottomWall = false;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 2:
                    if(CheckIsInBound(currentRoomX - 1, currentRoomY))
                    {
                        if(CheckLeftRoomIs(currentRoomNum, RoomType::None))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum - 1];

                            currentRoom->leftDoor = true;
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
                                nextRoom->rightWall = false;
                                currentRoom->maxDoorCount--;
                                currentRoom->leftWall = false;
                            }

                            RoomCreateAlgorithm(nextRoom, currentRoom);
                        }
                    }
                break;
                case 3:
                    if(CheckIsInBound(currentRoomX + 1, currentRoomY))
                    {
                        if(CheckRightRoomIs(currentRoomNum, RoomType::None))
                        {
                            FRoomInfo *nextRoom = &roomInfo[currentRoomNum + 1];

                            currentRoom->rightDoor = true;
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
                                nextRoom->leftWall = false;
                                currentRoom->maxDoorCount--;
                                currentRoom->rightWall = false;
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
    UE_LOG(LogTemp, Log, TEXT("Room Exit. door : %d, visited : %d, playerCount : %d"), currentRoom->maxDoorCount, visited.Num(), currentPlayerSpawn);
    return;
}

void AMainLevelScript::AddAnotherPlayerRoom()
{
    //이 함수 너무 드러워서 나중에 수정하기
    int startX = 2;
    int startY = 2;
    int index = 0;

    while(currentPlayerSpawn < maxPlayerSpawn && startY != 7)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, 2))
        {
            if(CheckLeftRoomIs(index, RoomType::corrider))
            {
                if(roomInfo[index - 1].maxDoorCount > 0)
                {
                    roomInfo[index].leftDoor = true;
                    roomInfo[index].maxDoorCount = 0;
                    roomInfo[index].roomType = RoomType::playerSpawn;
                    roomInfo[index - 1].rightDoor = true;
                    roomInfo[index - 1].maxDoorCount--;
                    currentPlayerSpawn++;
                    checkedList.Add(&roomInfo[index]);
                }
            }
        }
        startY++;
    }

    while(currentPlayerSpawn < maxPlayerSpawn && startX != 7)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, 2))
        {
            if(CheckLeftRoomIs(index, RoomType::corrider))
            {
                if(roomInfo[index + 9].maxDoorCount > 0)
                {
                    roomInfo[index].bottomDoor = true;
                    roomInfo[index].maxDoorCount = 0;
                    roomInfo[index].roomType = RoomType::playerSpawn;
                    roomInfo[index + 9].topDoor = true;
                    roomInfo[index + 9].maxDoorCount--;
                    currentPlayerSpawn++;
                    checkedList.Add(&roomInfo[index]);
                }
            }
        }
        startX++;
    }

    while(currentPlayerSpawn < maxPlayerSpawn && startY != 1)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, 2))
        {
            if(CheckLeftRoomIs(index, RoomType::corrider))
            {
                if(roomInfo[index + 1].maxDoorCount > 0)
                {
                    roomInfo[index].rightDoor = true;
                    roomInfo[index].maxDoorCount = 0;
                    roomInfo[index].roomType = RoomType::playerSpawn;
                    roomInfo[index + 1].leftDoor = true;
                    roomInfo[index + 1].maxDoorCount--;
                    currentPlayerSpawn++;
                    checkedList.Add(&roomInfo[index]);
                }
            }
        }
        startY--;
    }

    while(currentPlayerSpawn < maxPlayerSpawn && startX != 1)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, 2))
        {
            if(CheckLeftRoomIs(index, RoomType::corrider))
            {
                if(roomInfo[index - 9].maxDoorCount > 0)
                {
                    roomInfo[index].topDoor = true;
                    roomInfo[index].maxDoorCount = 0;
                    roomInfo[index].roomType = RoomType::playerSpawn;
                    roomInfo[index - 9].bottomDoor = true;
                    roomInfo[index - 9].maxDoorCount--;
                    currentPlayerSpawn++;
                    checkedList.Add(&roomInfo[index]);
                }
            }
        }
        startX--;
    }
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

bool AMainLevelScript::CheckTopRoomIs(int roomNum, RoomType type)
{
    if(roomNum / 9 == 0)
        return false;

    if(roomInfo[roomNum - 9].roomType == type)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckBottomRoomIs(int roomNum, RoomType type)
{
    if(roomNum / 9 == 8)
        return false;

    if(roomInfo[roomNum + 9].roomType == type)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckLeftRoomIs(int roomNum, RoomType type)
{
    if(roomNum % 9 == 0)
        return false;

    if(roomInfo[roomNum - 1].roomType == type)
        return true;
    else
        return false;
}

bool AMainLevelScript::CheckRightRoomIs(int roomNum, RoomType type)
{
    if(roomNum % 9 == 8)
        return false;

    if(roomInfo[roomNum + 1].roomType == type)
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