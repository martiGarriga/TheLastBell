#pragma once

#include <vector>
#include <memory>
#include "Room.h"
#include "Exit.h"
#include "NPC.h"
#include "Player.h"
#include "CommandReader.h"
#include "Item.h"
#include "GameClock.h"

namespace TimeCost 
{
    constexpr int PickUp = 5;
    constexpr int Drop = 2;
    constexpr int PutInside = 1;
    constexpr int Talk = 5;
    constexpr int Use = 3;
}


class World 
{
private:
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::unique_ptr<Exit>> exits;
    std::vector<std::unique_ptr<NPC>> npcs;
    std::unique_ptr<Player> player;

    Room* targetRoom;
    GameClock clock;

    bool isRunning;

    Room* CreateRoom(const std::string& name, const std::string& description);
    void ConnectRooms(Room* origin, Room* destination, Direction direction, bool bidirectional = true);
    NPC* CreateNPC(const std::string& name, const std::string& description, Room* location);
    Item* CreateItemInRoom(Room& room, const std::string& name, const std::string& description, bool canContainItems = false);

    void PrintCurrentRoom() const;
    void ExecuteCommand(const Command& command);
    void PrintHelp() const;

    void SpendTime(int minutes);
    void CheckEndConditions();

public:
    World();

    //Brain. Build the map, items, and NPCs, and place the player...
    void Init();

    void Run();

    bool IsRunning() const;
    void Stop();

    Player* GetPlayer() const;
};
