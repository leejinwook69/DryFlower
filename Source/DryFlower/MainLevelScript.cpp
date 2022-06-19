// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelScript.h"

void AMainLevelScript::InitialRoomSetup()
{
    //초기화
    roomInfo.Init(FRoomInfo(), 81);
    int roomNum = 99;
    int count = 0;
    //방번호 부여. 초기값 설정 필요하면 이곳에서
    for(auto& elem : roomInfo)
    {
        elem.SetWallAndDoor(true, true, true, true, false, false, false, false);
        elem.roomNumber = count;
        elem.maxDoorCount = 4;
        count++;
    }
    //-------------------------------------------------
    //EscapeRoom 우선 선정. 4개의 모서리 중 중복되지 않게 2개 선택
    //처음 생성된 EscapeRoom을 스타팅 룸 인덱스로 선정
    int escapeRoomIndex[4] = {0, 8, 72, 80};
    int startRoom = 99;
    int rand = 99;
    count = 0;

    while(count < 2)
    {
        int tempRand = FMath::RandRange(0, 3);
        if(rand != tempRand)
        {
            rand = tempRand;
            roomNum = escapeRoomIndex[rand];
            SetRoomType(roomNum, Arrow::None, RoomType::Escape);

            if(count == 0)
            {
                startRoom = roomNum;
                roomInfo[roomNum].maxDoorCount = 1;
            }

            count++;
        }
    }
    //-------------------------------------------------
    //EnemySpawnRoom 선정 (4, 4) 위치 고정
    roomNum = 40;
    SetRoomType(roomNum, Arrow::None, RoomType::EnemySpawn);
    //-------------------------------------------------
    //첫 알고리즘 시작 - 두께 2인 테두리 방에 플레이어 스폰 룸 생성
    UE_LOG(LogTemp, Log, TEXT("start room index : %d"), startRoom);
    RoomCreateAlgorithm(startRoom);
    //-------------------------------------------------
    //플레이어 스폰이 12개가 안됐을 경우, 추가 스폰 알고리즘
    if(currentPlayerSpawn < maxPlayerSpawn)
        AddAnotherPlayerRoom(3); //플레이어룸끼리 사이 거리 3으로 시도
    if(currentPlayerSpawn < maxPlayerSpawn)
        AddAnotherPlayerRoom(2); //아직도 12개가 안됐을 경우 거리 2로 재시도 (100퍼센트 12개 생성됨)
    //-------------------------------------------------
    //복도끼리 막히는 곳 없이 연결되어 있는지 확인하는 알고리즘 (반드시 한 곳은 막혀있어서 필요)
    ConnectingTest();
    //-------------------------------------------------
    //연구실 생성 알고리즘
    MakeRestrictedRoom();


    
    //UE_LOG(LogTemp, Log, TEXT("start room index : (%d, %d)"), startRoomNum.X, startRoomNum.Y);
}

void AMainLevelScript::RoomCreateAlgorithm(int currentRoomNum)
{
    TArray<int> visited;
    int maxDoorCount = roomInfo[currentRoomNum].maxDoorCount;

    while(true)
    {
        if(maxDoorCount <= 0)
            break;
        if(visited.Num() >= 4)
            break;

        int rand = FMath::RandRange(0, 3);
        if(visited.Contains(rand))
        {
            continue;
        }
        else
        {
            visited.Add(rand);
            Arrow arrow = Arrow::None;
            switch(rand)
            {
                case 0:
                    arrow = Arrow::Top;
                break;
                case 1:
                    arrow = Arrow::Bottom;
                break;
                case 2:
                    arrow = Arrow::Left;
                break;
                case 3:
                    arrow = Arrow::Right;
                break;
                default:
                    arrow = Arrow::None;
                break;
            }

            if(CheckIsInBound(currentRoomNum, arrow, 0, 8, 0, 8, minBorder))
            {
                int nextRoomNum = GetRoomNum(currentRoomNum, arrow);

                switch(GetRoomType(currentRoomNum, arrow))
                {
                    case RoomType::None:
                    {
                        if(CanMakePlayerSpawnRoom(currentRoomNum, arrow, 3))
                            SetRoomType(currentRoomNum, arrow, RoomType::PlayerSpawn);
                        else
                            SetRoomType(currentRoomNum, arrow, RoomType::Corrider);
                        
                        RoomCreateAlgorithm(nextRoomNum);
                    }
                    break;
                    case RoomType::Escape:
                        if(roomInfo[nextRoomNum].maxDoorCount >= 1)
                            SetRoomDoor(currentRoomNum, arrow, true);
                    break;
                    default:
                    break;
                }
            }
        }
    }
    return;
}

void AMainLevelScript::AddAnotherPlayerRoom(int distance)
{
    //이 함수 너무 드러워서 나중에 수정하기
    int startX = 2;
    int startY = 2;
    int index = 0;

    while(currentPlayerSpawn < maxPlayerSpawn)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, Arrow::None, distance))
        {
            if(GetRoomType(index, Arrow::Left) == RoomType::Corrider)
            {
                FRoomInfo *nextRoom = &roomInfo[GetRoomNum(index, Arrow::Left)];

                if(nextRoom->maxDoorCount > 1)
                {
                    SetRoomType(nextRoom->roomNumber, Arrow::Right, RoomType::PlayerSpawn);
                }
            }
        }

        if(startY == 6)
            break;

        startY++;
    }
    while(currentPlayerSpawn < maxPlayerSpawn)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, Arrow::None, distance))
        {
            if(GetRoomType(index, Arrow::Bottom) == RoomType::Corrider)
            {
                FRoomInfo *nextRoom = &roomInfo[GetRoomNum(index, Arrow::Bottom)];

                if(nextRoom->maxDoorCount > 1)
                {
                    SetRoomType(nextRoom->roomNumber, Arrow::Top, RoomType::PlayerSpawn);
                }
            }
        }
        
        if(startX == 6)
            break;

        startX++;
    }

    while(currentPlayerSpawn < maxPlayerSpawn)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, Arrow::None, distance))
        {
            if(GetRoomType(index, Arrow::Right) == RoomType::Corrider)
            {
                FRoomInfo *nextRoom = &roomInfo[GetRoomNum(index, Arrow::Right)];

                if(nextRoom->maxDoorCount > 1)
                {
                    SetRoomType(nextRoom->roomNumber, Arrow::Left, RoomType::PlayerSpawn);
                }
            }
        }

        if(startY == 2)
            break;

        startY--;
    }

    while(currentPlayerSpawn < maxPlayerSpawn)
    {
        index = startY * 9 + startX;

        if(CanMakePlayerSpawnRoom(index, Arrow::None, distance))
        {
            if(GetRoomType(index, Arrow::Top) == RoomType::Corrider)
            {
                FRoomInfo *nextRoom = &roomInfo[GetRoomNum(index, Arrow::Top)];

                if(nextRoom->maxDoorCount > 1)
                {
                    SetRoomType(nextRoom->roomNumber, Arrow::Bottom, RoomType::PlayerSpawn);
                }
            }
        }
        
        if(startX == 2)
            break;

        startX--;
    }
}

void AMainLevelScript::ConnectingTest()
{
    bool checkList[81] = {false};

    int maxCurrentRoomNum = -1;
    Arrow maxDestinationRoomArrow = Arrow::None;
    int maxDistance = -1;
    
    for(auto& elem : roomInfo)
    {
        if(checkList[elem.roomNumber])
            continue;

        if(elem.roomType == RoomType::Corrider && elem.maxDoorCount >= 2)
        {
            Arrow arrow[4] = {Arrow::Top, Arrow::Bottom, Arrow::Left, Arrow::Right};
            int currentRoomNum = elem.roomNumber;

            for(int i = 0; i < 4; i++)
            {
                if(CheckIsInBound(currentRoomNum, arrow[i], 0, 8, 0, 8, minBorder))
                {
                    int destinationRoomNum = GetRoomNum(currentRoomNum, arrow[i]);

                    if(GetRoomType(currentRoomNum, arrow[i]) == RoomType::Corrider && CheckWallExist(currentRoomNum, arrow[i]))
                    {   
                        int distance = GetDestinationDistance(currentRoomNum, destinationRoomNum, 99);
                        
                        if(distance > maxDistance)
                        {
                            maxCurrentRoomNum = currentRoomNum;
                            maxDestinationRoomArrow = arrow[i];
                            maxDistance = distance;
                        }

                        checkList[currentRoomNum] = true;
                        checkList[destinationRoomNum] = true;
                    }
                }
            }
        }
    }
    SetRoomWall(maxCurrentRoomNum, maxDestinationRoomArrow, false);
    SetRoomDoor(maxCurrentRoomNum, maxDestinationRoomArrow, true);
}

int AMainLevelScript::GetDestinationDistance(int currentRoomNum, int destinationRoomNum, int beforeRoomNum)
{
    if(currentRoomNum < 0 || currentRoomNum > 80)
        return 0;

    if(currentRoomNum == destinationRoomNum)
        return 1;

    Arrow arrow[4] = {Arrow::Top, Arrow::Bottom, Arrow::Left, Arrow::Right};

    for(int i = 0; i < 4; i++)
    {
        if(CheckIsInBound(currentRoomNum, arrow[i], 0, 8, 0, 8, minBorder))
        {
            int nextRoomNum = GetRoomNum(currentRoomNum, arrow[i]);

            if(nextRoomNum == beforeRoomNum)
                continue;

            if(GetRoomType(currentRoomNum, arrow[i]) == RoomType::Corrider)
            {
                if(CheckWallExist(currentRoomNum, arrow[i]))
                    continue;
                else
                {
                    int distance = GetDestinationDistance(nextRoomNum, destinationRoomNum, currentRoomNum);

                    if(distance > 0)
                        return distance + 1;
                }
            }
        }
    }
    return 0;
}

void AMainLevelScript::MakeRestrictedRoom()
{
    TArray<int> makedRestrictedIndex; //1, 2, 3 = col, 4, 5 = row
    TArray<int> makedRestrictedRoomNumber;

    //바깥 공간에 제한 구역을 전부 만드는 경우의 수는 없을 것 같아서, 방어 처리는 안되어있음
    for(auto& elem : roomInfo)
    {
        Arrow arrow;
        if(elem.roomType == RoomType::Corrider && elem.maxDoorCount >= 3)
        {
            if(elem.topWall == false)
                arrow = Arrow::Top;
            else if(elem.bottomWall == false)
                arrow = Arrow::Bottom;
            else if(elem.leftWall == false)
                arrow = Arrow::Left;
            else
                arrow = Arrow::Right;
            
            int nextRoomNum = GetRoomNum(elem.roomNumber, arrow);
            if(CheckIsInBound(nextRoomNum, Arrow::None, 0, 8, 0, 8, 2))
            {
                FRoomInfo *nextRoom = &roomInfo[nextRoomNum];
                if(nextRoom->roomType == RoomType::Corrider && nextRoom->maxDoorCount >= 2)
                {
                    if(arrow == Arrow::Top || arrow == Arrow::Bottom)
                    {
                        while(true)
                        {
                            int rand = FMath::RandRange(1, 3);
                            if(makedRestrictedIndex.Contains(rand))
                                continue;
                            else
                            {
                                switch(rand)
                                {
                                    case 1:
                                        SetRoomType(elem.roomNumber, arrow, RoomType::Restricted01_0);
                                    break;
                                    case 2:
                                        SetRoomType(elem.roomNumber, arrow, RoomType::Restricted02_0);
                                    break;
                                    case 3:
                                        SetRoomType(elem.roomNumber, arrow, RoomType::Restricted03_0);
                                    break;
                                    default:
                                    break;
                                }
                                makedRestrictedIndex.Add(rand);
                                makedRestrictedRoomNumber.Add(elem.roomNumber);
                                break;
                            }
                        }
                    }
                    else
                    {
                        while(true)
                        {
                            int rand = FMath::RandRange(4, 5);
                            if(makedRestrictedIndex.Contains(rand))
                                continue;
                            else
                            {
                                switch(rand)
                                {
                                    case 4:
                                        SetRoomType(elem.roomNumber, arrow, RoomType::Restricted04_0);
                                    break;
                                    case 5:
                                        SetRoomType(elem.roomNumber, arrow, RoomType::Restricted05_0);
                                    break;
                                    default:
                                    break;
                                }
                                makedRestrictedIndex.Add(rand);
                                makedRestrictedRoomNumber.Add(elem.roomNumber);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    Arrow arrowArr[4] = {Arrow::Top, Arrow::Bottom, Arrow::Left, Arrow::Right};
                    arrow = Arrow::None;

                    for(int i = 0; i < 4; i++)
                    {
                        if(CheckIsInBound(elem.roomNumber, arrowArr[i], 0, 8, 0, 8, 9))
                        {
                            if(GetRoomType(elem.roomNumber, arrowArr[i]) == RoomType::None)
                            {
                                arrow = arrowArr[i];
                                break;
                            }
                        }
                    }

                    if(arrow != Arrow::None)
                    {
                        if(arrow == Arrow::Top || arrow == Arrow::Bottom)
                        {
                            while(true)
                            {
                                int rand = FMath::RandRange(1, 3);
                                if(makedRestrictedIndex.Contains(rand))
                                    continue;
                                else
                                {
                                    switch(rand)
                                    {
                                        case 1:
                                            SetRoomType(elem.roomNumber, arrow, RoomType::Restricted01_0);
                                        break;
                                        case 2:
                                            SetRoomType(elem.roomNumber, arrow, RoomType::Restricted02_0);
                                        break;
                                        case 3:
                                            SetRoomType(elem.roomNumber, arrow, RoomType::Restricted03_0);
                                        break;
                                        default:
                                        break;
                                    }
                                    makedRestrictedIndex.Add(rand);
                                    makedRestrictedRoomNumber.Add(elem.roomNumber);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            while(true)
                            {
                                int rand = FMath::RandRange(4, 5);
                                if(makedRestrictedIndex.Contains(rand))
                                    continue;
                                else
                                {
                                    switch(rand)
                                    {
                                        case 4:
                                            SetRoomType(elem.roomNumber, arrow, RoomType::Restricted04_0);
                                        break;
                                        case 5:
                                            SetRoomType(elem.roomNumber, arrow, RoomType::Restricted05_0);
                                        break;
                                        default:
                                        break;
                                    }
                                    makedRestrictedIndex.Add(rand);
                                    makedRestrictedRoomNumber.Add(elem.roomNumber);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //가장 큰 부피를 차지하는 연구소 방 만들기
    while(true)
    {
        int rand = FMath::RandRange(20, 50);

        if(GetRoomType(rand, Arrow::None) != RoomType::None)
            continue;
        
        if(CheckIsInBound(rand, Arrow::None, 2, 5, 2, 5, 1) == false)
            continue;

        if(GetRoomType(rand, Arrow::Right) != RoomType::None || GetRoomType(rand, Arrow::Bottom) != RoomType::None || 
                GetRoomType(GetRoomNum(rand, Arrow::Bottom), Arrow::Right) != RoomType::None)
            continue;
        
        SetRoomType(rand, Arrow::None, RoomType::Labatory_0);
        break;
    }

    //제한구역 1, 2, 3 중 안만들어진 방 만들기
    for(int i = 1; i <= 3; i++)
    {
        if(makedRestrictedIndex.Contains(i))
            continue;

        while(true)
        {
            int rand = FMath::RandRange(20, 60);

            if(GetRoomType(rand, Arrow::None) != RoomType::None)
                continue;

            if(GetRoomType(rand, Arrow::Bottom) != RoomType::None)
                continue;
            
            switch(i)
            {
                case 1:
                    SetRoomType(rand, Arrow::Bottom, RoomType::Restricted01_0);
                break;
                case 2:
                    SetRoomType(rand, Arrow::Bottom, RoomType::Restricted02_0);
                break;
                case 3:
                    SetRoomType(rand, Arrow::Bottom, RoomType::Restricted03_0);
                break;
                default:
                break;
            }
            makedRestrictedIndex.Add(i);
            makedRestrictedRoomNumber.Add(rand);
            break;
        }
    }

    //제한구역 4, 5 중 안만들어진 방 만들기
    for(int i = 4; i <= 5; i++)
    {
        if(makedRestrictedIndex.Contains(i))
            continue;

        while(true)
        {
            int rand = FMath::RandRange(20, 60);

            if(GetRoomType(rand, Arrow::None) != RoomType::None)
                continue;

            if(GetRoomType(rand, Arrow::Right) != RoomType::None)
                continue;
            
            switch(i)
            {
                case 4:
                    SetRoomType(rand, Arrow::Right, RoomType::Restricted04_0);
                break;
                case 5:
                    SetRoomType(rand, Arrow::Right, RoomType::Restricted05_0);
                break;
                default:
                break;
            }
            makedRestrictedIndex.Add(i);
            makedRestrictedRoomNumber.Add(rand);
            break;
        }
    }

    //제한구역 순회하면서 문 / 벽 만들어주기
    int count = 0;
    for(auto elem : makedRestrictedRoomNumber)
    {
        UE_LOG(LogTemp, Log, TEXT("###makedRestRoomNum[%d] : %d, index : %d"), count, elem, makedRestrictedIndex[count]);
        RoomType roomType = GetRoomType(elem, Arrow::None);

        if(makedRestrictedIndex[count] <= 3)
        {
            if(roomType == RoomType::Restricted01_0 || roomType == RoomType::Restricted02_0 || roomType == RoomType::Restricted03_0)
                SetRestrictedWallAndDoor(elem, elem + 9, Arrow::Bottom);
            else
                SetRestrictedWallAndDoor(elem, elem - 9, Arrow::Top);
        }
        else
        {
            if(roomType == RoomType::Restricted04_0 || roomType == RoomType::Restricted05_0)
                SetRestrictedWallAndDoor(elem, elem + 1, Arrow::Right);
            else
                SetRestrictedWallAndDoor(elem, elem - 1, Arrow::Left);
        }
        count++;
    }

    //마지막으로 순회하면서 남은 공간 전부 복도로 만들기, 벽 뚫고 문 만들기
    for(auto& elem : roomInfo)
    {
        if(elem.roomType == RoomType::None)
        {
            Arrow arrowArr[4] = {Arrow::Top, Arrow::Bottom, Arrow::Left, Arrow::Right};
            
            SetRoomType(elem.roomNumber, Arrow::None, RoomType::Corrider);
            for(int i = 0; i < 4; i++)
            {
                RoomType roomType = GetRoomType(elem.roomNumber, arrowArr[i]);
                if(roomType == RoomType::Corrider)
                    SetRoomWall(elem.roomNumber, arrowArr[i], false);
                else if(roomType == RoomType::EnemySpawn || roomType == RoomType::Labatory_0 || roomType == RoomType::Labatory_1 || roomType == RoomType::Labatory_2 ||
                            roomType == RoomType::Labatory_3)
                    SetRoomDoor(elem.roomNumber, arrowArr[i], true);
            }

        }
    }
}

bool AMainLevelScript::CheckIsInBound(int roomIndex, Arrow arrow, int minX, int maxX, int minY, int maxY, int width)
{
    if(roomIndex < 0 || roomIndex > 80)
        return false;

    int roomX = roomIndex % 9;
    int roomY = roomIndex / 9;

    switch(arrow)
    {
        case Arrow::Top:
            roomY--;
        break;
        case Arrow::Bottom:
            roomY++;
        break;
        case Arrow::Left:
            roomX--;
        break;
        case Arrow::Right:
            roomX++;
        break;
        default:
        break;
    }

    if(roomX < minX || roomX > maxX || roomY < minY || roomY > maxY)
        return false;
    else if(roomX >= minX + width && roomX <= maxX - width && roomY >= minY + width && roomY <= maxY - width)
        return false;
    else
        return true;
}

bool AMainLevelScript::CanMakePlayerSpawnRoom(int currentRoomNum, Arrow arrow, int distance)
{
    int targetRoomNum = GetRoomNum(currentRoomNum, arrow);

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

int AMainLevelScript::GetRoomNum(int currentRoomNum, Arrow arrow)
{
    switch(arrow)
    {
        case Arrow::Top:
            return currentRoomNum - 9;
        break;
        case Arrow::Bottom:
            return currentRoomNum + 9;
        break;
        case Arrow::Left:
            return currentRoomNum - 1;
        break;
        case Arrow::Right:
            return currentRoomNum + 1;
        break;
        default:
            return currentRoomNum;
        break;
    }
}

RoomType AMainLevelScript::GetRoomType(int currentRoomNum, Arrow arrow)
{
    return roomInfo[GetRoomNum(currentRoomNum, arrow)].roomType;
}

void AMainLevelScript::SetRoomType(int currentRoomNum, Arrow arrow, RoomType roomType)
{
    FRoomInfo *currentRoom = &roomInfo[currentRoomNum];
    FRoomInfo *nextRoom = &roomInfo[GetRoomNum(currentRoomNum, arrow)];

    switch(roomType)
    {
        case RoomType::Escape:
            currentRoom->roomType = RoomType::Escape;
            currentRoom->maxDoorCount = 1;
            checkedList.Add(currentRoom);
        break;
        case RoomType::EnemySpawn:
            currentRoom->roomType = RoomType::EnemySpawn;
        break;
        case RoomType::PlayerSpawn:
            SetRoomDoor(currentRoomNum, arrow, true);
            currentPlayerSpawn++;
            nextRoom->maxDoorCount = 0;
            nextRoom->roomType = RoomType::PlayerSpawn;
            checkedList.Add(nextRoom);
        break;
        case RoomType::Corrider:
            if(currentRoom->roomType != RoomType::Escape)
                SetRoomWall(currentRoomNum, arrow, false);
            SetRoomDoor(currentRoomNum, arrow, true);
            nextRoom->roomType = RoomType::Corrider;
        break;
        case RoomType::Restricted01_0:
            if(currentRoom->roomNumber < nextRoom->roomNumber)
            {
                currentRoom->roomType = roomType;
                nextRoom->roomType = RoomType::Restricted01_1;
            }
            else
            {
                nextRoom->roomType = roomType;
                currentRoom->roomType = RoomType::Restricted01_1;
            }
        break;
        case RoomType::Restricted02_0:
            if(currentRoom->roomNumber < nextRoom->roomNumber)
            {
                currentRoom->roomType = roomType;
                nextRoom->roomType = RoomType::Restricted02_1;
            }
            else
            {
                nextRoom->roomType = roomType;
                currentRoom->roomType = RoomType::Restricted02_1;
            }
        break;
        case RoomType::Restricted03_0:
            if(currentRoom->roomNumber < nextRoom->roomNumber)
            {
                currentRoom->roomType = roomType;
                nextRoom->roomType = RoomType::Restricted03_1;
            }
            else
            {
                nextRoom->roomType = roomType;
                currentRoom->roomType = RoomType::Restricted03_1;
            }
        break;
        case RoomType::Restricted04_0:
            if(currentRoom->roomNumber < nextRoom->roomNumber)
            {
                currentRoom->roomType = roomType;
                nextRoom->roomType = RoomType::Restricted04_1;
            }
            else
            {
                nextRoom->roomType = roomType;
                currentRoom->roomType = RoomType::Restricted04_1;
            }
        break;
        case RoomType::Restricted05_0:
            if(currentRoom->roomNumber < nextRoom->roomNumber)
            {
                currentRoom->roomType = roomType;
                nextRoom->roomType = RoomType::Restricted05_1;
            }
            else
            {
                nextRoom->roomType = roomType;
                currentRoom->roomType = RoomType::Restricted05_1;
            }
        break;
        case RoomType::Labatory_0:
            currentRoom->roomType = roomType;
            SetRoomWall(currentRoom->roomNumber, Arrow::Right, false);
            SetRoomWall(currentRoom->roomNumber, Arrow::Bottom, false);
            nextRoom = &roomInfo[currentRoom->roomNumber + 1];
            nextRoom->roomType = RoomType::Labatory_1;
            SetRoomWall(nextRoom->roomNumber, Arrow::Bottom, false);
            nextRoom = &roomInfo[currentRoom->roomNumber + 9];
            nextRoom->roomType = RoomType::Labatory_2;
            SetRoomWall(nextRoom->roomNumber, Arrow::Right, false);
            nextRoom = &roomInfo[nextRoom->roomNumber + 1];
            nextRoom->roomType = RoomType::Labatory_3;
            //문 만드는건 제한구역 전부 만들어진 다음에 해야할듯
        break;
        default:
        break;
    }
}

void AMainLevelScript::SetRestrictedWallAndDoor(int currentRoomNum, int nextRoomNum, Arrow arrow)
{
    switch(arrow)
    {
        case Arrow::Top:
            SetRoomWall(currentRoomNum, Arrow::Top, false);
            SetRoomWall(currentRoomNum, Arrow::Bottom, true);
            SetRoomWall(currentRoomNum, Arrow::Left, true);
            SetRoomWall(currentRoomNum, Arrow::Right, true);

            SetRoomWall(nextRoomNum, Arrow::Top, true);
            SetRoomWall(nextRoomNum, Arrow::Bottom, false);
            SetRoomWall(nextRoomNum, Arrow::Left, true);
            SetRoomWall(nextRoomNum, Arrow::Right, true);
        break;
        case Arrow::Bottom:
            SetRoomWall(currentRoomNum, Arrow::Top, true);
            SetRoomWall(currentRoomNum, Arrow::Bottom, false);
            SetRoomWall(currentRoomNum, Arrow::Left, true);
            SetRoomWall(currentRoomNum, Arrow::Right, true);

            SetRoomWall(nextRoomNum, Arrow::Top, false);
            SetRoomWall(nextRoomNum, Arrow::Bottom, true);
            SetRoomWall(nextRoomNum, Arrow::Left, true);
            SetRoomWall(nextRoomNum, Arrow::Right, true);
        break;
        case Arrow::Left:
            SetRoomWall(currentRoomNum, Arrow::Top, true);
            SetRoomWall(currentRoomNum, Arrow::Bottom, true);
            SetRoomWall(currentRoomNum, Arrow::Left, false);
            SetRoomWall(currentRoomNum, Arrow::Right, true);

            SetRoomWall(nextRoomNum, Arrow::Top, true);
            SetRoomWall(nextRoomNum, Arrow::Bottom, true);
            SetRoomWall(nextRoomNum, Arrow::Left, true);
            SetRoomWall(nextRoomNum, Arrow::Right, false);
        break;
        case Arrow::Right:
            SetRoomWall(currentRoomNum, Arrow::Top, true);
            SetRoomWall(currentRoomNum, Arrow::Bottom, true);
            SetRoomWall(currentRoomNum, Arrow::Left, true);
            SetRoomWall(currentRoomNum, Arrow::Right, false);

            SetRoomWall(nextRoomNum, Arrow::Top, true);
            SetRoomWall(nextRoomNum, Arrow::Bottom, true);
            SetRoomWall(nextRoomNum, Arrow::Left, false);
            SetRoomWall(nextRoomNum, Arrow::Right, true);
        break;
        default:
        break;
    }

    Arrow arrowArr[4] = {Arrow::Top, Arrow::Bottom, Arrow::Left, Arrow::Right};
    for(int i = 0; i < 4; i++)
    {
        if(CheckIsInBound(currentRoomNum, arrowArr[i], 0, 8, 0, 8, 9) == false)
            continue;

        RoomType currentRoomType = GetRoomType(currentRoomNum, arrowArr[i]);
        if(currentRoomType == RoomType::Corrider || /*currentRoomType == RoomType::Labatory ||*/ currentRoomType == RoomType::EnemySpawn || currentRoomType == RoomType::None)
        {
            SetRoomDoor(currentRoomNum, arrowArr[i], true);
            break;
        }
    }

    for(int i = 0; i < 4; i++)
    {
        if(CheckIsInBound(nextRoomNum, arrowArr[i], 0, 8, 0, 8, 9) == false)
            continue;

        RoomType nextRoomType = GetRoomType(nextRoomNum, arrowArr[i]);
        if(nextRoomType == RoomType::Corrider || /*nextRoomType == RoomType::Labatory ||*/ nextRoomType == RoomType::EnemySpawn || nextRoomType == RoomType::None)
        {
            SetRoomDoor(nextRoomNum, arrowArr[i], true);
            break;
        }
    }
}

void AMainLevelScript::SetRoomDoor(int currentRoomNum, Arrow arrow, bool active)
{
    FRoomInfo *currentRoom = &roomInfo[currentRoomNum];
    FRoomInfo *nextRoom = &roomInfo[GetRoomNum(currentRoomNum, arrow)];

    switch(arrow)
    {
        case Arrow::Top:
            currentRoom->topDoor = active;
            nextRoom->bottomDoor = active;
        break;
        case Arrow::Bottom:
            currentRoom->bottomDoor = active;
            nextRoom->topDoor = active;
        break;
        case Arrow::Left:
            currentRoom->leftDoor = active;
            nextRoom->rightDoor = active;
        break;
        case Arrow::Right:
            currentRoom->rightDoor = active;
            nextRoom->leftDoor = active;
        break;
        default:
        break;
    }

    if(arrow != Arrow::None)
    {
        if(active)
        {
            currentRoom->maxDoorCount--;
            nextRoom->maxDoorCount--;
        }
        else
        {
            currentRoom->maxDoorCount++;
            nextRoom->maxDoorCount++;
        }
    }
}

void AMainLevelScript::SetRoomWall(int currentRoomNum, Arrow arrow, bool active)
{
    if(CheckIsInBound(currentRoomNum, arrow, 0, 8, 0, 8, 9) == false)
        return;

    FRoomInfo *currentRoom = &roomInfo[currentRoomNum];
    FRoomInfo *nextRoom = &roomInfo[GetRoomNum(currentRoomNum, arrow)];

    switch(arrow)
    {
        case Arrow::Top:
            currentRoom->topWall = active;
            nextRoom->bottomWall = active;
        break;
        case Arrow::Bottom:
            currentRoom->bottomWall = active;
            nextRoom->topWall = active;
        break;
        case Arrow::Left:
            currentRoom->leftWall = active;
            nextRoom->rightWall = active;
        break;
        case Arrow::Right:
            currentRoom->rightWall = active;
            nextRoom->leftWall = active;
        break;
        default:
        break;
    }
}

bool AMainLevelScript::CheckWallExist(int currentRoomNum, Arrow arrow)
{
    FRoomInfo *currentRoom = &roomInfo[currentRoomNum];

    switch(arrow)
    {
        case Arrow::Top:
            return currentRoom->topWall;
        break;
        case Arrow::Bottom:
            return currentRoom->bottomWall;
        break;
        case Arrow::Left:
            return currentRoom->leftWall;
        break;
        case Arrow::Right:
            return currentRoom->rightWall;
        break;
        default:
            return false;
        break;
    }
}

//리플리케이션 관련 함수
void AMainLevelScript::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMainLevelScript, corriderRoom);
    DOREPLIFETIME(AMainLevelScript, playerSpawnRoom);
    DOREPLIFETIME(AMainLevelScript, enemySpawnRoom);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom01_0);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom01_1);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom02_0);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom02_1);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom03_0);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom03_1);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom04_0);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom04_1);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom05_0);
    DOREPLIFETIME(AMainLevelScript, restrictedRoom05_1);
    DOREPLIFETIME(AMainLevelScript, escapeRoom);
    DOREPLIFETIME(AMainLevelScript, officeRoom_0);
    DOREPLIFETIME(AMainLevelScript, officeRoom_1);
    DOREPLIFETIME(AMainLevelScript, officeRoom_2);
    DOREPLIFETIME(AMainLevelScript, labatoryRoom_0);
    DOREPLIFETIME(AMainLevelScript, labatoryRoom_1);
    DOREPLIFETIME(AMainLevelScript, labatoryRoom_2);
    DOREPLIFETIME(AMainLevelScript, labatoryRoom_3);
    DOREPLIFETIME(AMainLevelScript, loungeRoom_0);
    DOREPLIFETIME(AMainLevelScript, loungeRoom_1);
    DOREPLIFETIME(AMainLevelScript, roomInfo);
    DOREPLIFETIME(AMainLevelScript, maxPlayerSpawn);
    DOREPLIFETIME(AMainLevelScript, currentPlayerSpawn);
    //DOREPLIFETIME(AMainLevelScript, checkedList);
    DOREPLIFETIME(AMainLevelScript, minBorder);
    DOREPLIFETIME(AMainLevelScript, pawnList);
    DOREPLIFETIME(AMainLevelScript, maxBorder);
}