#include "World.h"
#include <iostream>

World::World()
    : targetRoom(nullptr)
    , clock(1440)
    , isRunning(false)
{
}

Room* World::CreateRoom(const std::string& name, const std::string& description)
{
    rooms.push_back(std::make_unique<Room>(name, description));
    return rooms.back().get();
}

void World::ConnectRooms(Room* source, Room* destination, Direction direction, bool bidirectional)
{
    exits.push_back(std::make_unique<Exit>(direction, source, destination));
    source->AddExit(exits.back().get());

    if (bidirectional)
    {
        Direction opposite;
        switch (direction)
        {
        case Direction::North: opposite = Direction::South; break;
        case Direction::South: opposite = Direction::North; break;
        case Direction::East:  opposite = Direction::West;  break;
        case Direction::West:  opposite = Direction::East;  break;
        case Direction::Up:    opposite = Direction::Down;  break;
        case Direction::Down:  opposite = Direction::Up;    break;
        default:              opposite = Direction::North; break;
        }
        exits.push_back(std::make_unique<Exit>(opposite, destination, source));
        destination->AddExit(exits.back().get());
    }
}

NPC* World::CreateNPC(const std::string& name, const std::string& description, Room* location)
{
    npcs.push_back(std::make_unique<NPC>(name, description, location));
    return npcs.back().get();
}

Item* World::CreateItemInRoom(Room& room, const std::string& name, const std::string& description, bool canContainItems)
{
    auto item = std::make_unique<Item>(name, description, canContainItems);
    Item* itemPtr = item.get();
    room.AddEntity(std::move(item));
    return itemPtr;
}

void World::Init()
{
    //Zones
    Room* clearing = CreateRoom("The Clearing", "A quiet clearing at the edge of an ancient forest. The last light of evening is fading.");
    Room* forest = CreateRoom("Whispering Forest", "Tall trees surround a narrow path. A cold wind moves through the branches. A CROW watches you intently.");
    Room* village = CreateRoom("Valdren Village", "A small, abandoned village, apparently with only one VILLAGER sitting in the doorway of a house. An old gate to the east leads to a forgotten shrine.");
    Room* sanctuary = CreateRoom("Old Sanctuary", "Broken stone pillars surround a statue of the old king. Everything here feels untouched for centuries.");
    Room* cave = CreateRoom("Echoing Cave", "A dark cave descends beneath the hill. You can barely see your own hands.");
    Room* bridge = CreateRoom("Stone Bridge", "A narrow stone bridge crosses a deep ravine. A faint magical barrier blocks the road ahead.");
    Room* tower = CreateRoom("The Tower", "A tall tower rises above Valdren. At its top stands a great door glowing with pale light.");
    Room* exitRoom = CreateRoom("Dawn Gate", "The ancient Dawn Gate stands open, revealing the first light beyond Valdren.");

    //Connections
    ConnectRooms(clearing, forest, Direction::North);
    ConnectRooms(forest, village, Direction::North);

    //Uses
    exits.push_back(std::make_unique<Exit>(Direction::East, village, sanctuary, true, "rusty key"));
    village->AddExit(exits.back().get());
    exits.push_back(std::make_unique<Exit>(Direction::West, sanctuary, village));
    sanctuary->AddExit(exits.back().get());

    //Connections
    ConnectRooms(sanctuary, cave, Direction::Down);
    ConnectRooms(sanctuary, bridge, Direction::East);

    //Uses
    exits.push_back(std::make_unique<Exit>(Direction::North, bridge, tower, true, "stone"));
    bridge->AddExit(exits.back().get());
    exits.push_back(std::make_unique<Exit>(Direction::South, tower, bridge));
    tower->AddExit(exits.back().get());

    exits.push_back(std::make_unique<Exit>(Direction::Up, tower, exitRoom, true, "stone"));
    tower->AddExit(exits.back().get());
    exits.push_back(std::make_unique<Exit>(Direction::Down, exitRoom, tower));
    exitRoom->AddExit(exits.back().get());

    //Objects
    CreateItemInRoom(*clearing, "letter", "A short letter: 'Reach the Dawn Gate before midnight.'");
    CreateItemInRoom(*clearing, "backpack", "An old but useful backpack for the trip", true);
    CreateItemInRoom(*village, "rusty key", "An old iron key covered in rust.");
    CreateItemInRoom(*village, "lantern", "A small oil lantern. It still has enough oil to burn.");

    //NPC
    NPC* crow = CreateNPC("crow", "A black crow watches you from a low branch.", forest);
    crow->AddDialogueLine("Crow: The bells will ring twelve times tonight.");
    crow->AddDialogueLine("Crow: The old sanctuary lies beyond the village gate.");
    crow->AddDialogueLine("Crow: The guardian still waits beneath the hill.");

    NPC* villager = CreateNPC("villager", "An old villager sits silently beside an abandoned house.", village);
    villager->AddDialogueLine("Villager: The Dawn Gate is at the top of the tower.");
    villager->AddDialogueLine("Villager: The sanctuary gate has been locked for years.");
    villager->AddDialogueLine("Villager: I think the old key is still somewhere in this village.");

    NPC* guardian = CreateNPC("guardian", "An old guardian waits in the darkness beneath the sanctuary.", cave);
    guardian->AddDialogueLine("Guardian: You came before the final bell.");
    guardian->AddDialogueLine("Guardian: The king hide a stone in the sanctuary.");
    guardian->AddDialogueLine("Guardian: Bring that light to the kings statue and use the power of the stone to reach the Dawn Gate.");

    targetRoom = exitRoom;
    player = std::make_unique<Player>("player", clearing);
    isRunning = true;

    std::cout << "=== THE LAST BELL ===\n";
    std::cout << "Valdren will be frozen forever when the clock reaches midnight.\n";
    std::cout << "Find a way to open the Dawn Gate before 00:00.\n";
    std::cout << "Every movement, item, action and conversation costs time.\n";
    std::cout << "Type 'help' for the available commands.\n";
    std::cout << "Characters written in uppercase can be interacted with using the 'talk' command.\n";
    std::cout << "Start time: " << clock.GetHMTime() << "\n\n";
}

void World::PrintCurrentRoom() const
{
    std::cout << "\n[Time: " << clock.GetHMTime() << " | " << clock.GetRemainingMinutes() << " min remaining]\n";
    std::cout << "\n" << player->GetLocation()->GetFullDescription() << "\n";
}

void World::PrintHelp() const
{
    std::cout << "Commands:\n"
        << "  go <direction>            - north, south, east, west, up, down\n"
        << "  take <item>               - pick up an item\n"
        << "  drop <item>               - drop an item\n"
        << "  put <item> in <container> - store an item inside another item\n"
        << "  use <item>                - use an item in the current room\n"
        << "  talk <npc>                - talk to a character\n"
        << "  inventory                 - show your inventory\n"
        << "  look                      - look around\n"
        << "  help                      - show commands\n"
        << "  exit                      - quit the game\n";
}

void World::SpendTime(int minutes)
{
    clock.Advance(minutes);
    std::cout << "(Action took " << minutes << " min. It is now " << clock.GetHMTime() << ")\n";
    CheckEndConditions();
}

void World::CheckEndConditions()
{
    if (!isRunning)
        return;

    if (player->GetLocation() == targetRoom)
    {
        std::cout << "\nYou reach the Dawn Gate at " << clock.GetHMTime() << ".\n";
        std::cout << "You step beyond Valdren just before the final bell.\n";
        std::cout << "YOU ESCAPED. VICTORY!\n";
        Stop();
        return;
    }

    if (clock.HasExpired())
    {
        std::cout << "\nMidnight arrives. The twelfth bell rings across Valdren.\n";
        std::cout << "Everything becomes still.\n";
        std::cout << "TIME HAS RUN OUT. DEFEAT.\n";
        Stop();
    }
}

void World::ExecuteCommand(const Command& command)
{
    if (command.verb.empty())
    {
        std::cout << "I don't understand.\n";
        return;
    }

    if (command.verb == "help")
    {
        PrintHelp();
    }
    else if (command.verb == "look")
    {
        PrintCurrentRoom();
    }
    else if (command.verb == "inventory")
    {
        player->ListInventory();
    }
    else if (command.verb == "exit" && command.target.empty())
    {
        std::cout << "Goodbye.\n";
        Stop();
    }
    else if (command.verb == "go" || command.verb == "move")
    {
        Direction direction;
        if (!Exit::TryReadDirection(command.target, direction))
        {
            std::cout << "Unknown direction.\n";
            return;
        }

        Exit* exit = player->GetLocation()->GetExit(direction);
        if (exit == nullptr)
        {
            std::cout << "You cannot go that way.\n";
            return;
        }

        if (exit->IsLocked())
        {
            std::cout << "That way is closed. Use the required item here first.\n";
            return;
        }

        player->MoveTo(exit->GetDestination());
        SpendTime(exit->GetTimeCost());
        if (isRunning)
            PrintCurrentRoom();
    }
    else if (command.verb == "pickup" || command.verb == "take")
    {
        if (player->PickUp(command.target))
            SpendTime(TimeCost::PickUp);
    }
    else if (command.verb == "drop")
    {
        if (player->Drop(command.target))
            SpendTime(TimeCost::Drop);
    }
    else if (command.verb == "put")
    {
        if (player->PutInside(command.target, command.targetTwo))
            SpendTime(TimeCost::PutInside);
    }
    else if (command.verb == "use")
    {
        const std::string& itemName = command.target;
        if (player->FindEntity(itemName) == nullptr)
        {
            std::cout << "You are not carrying a \"" << itemName << "\".\n";
            return;
        }

        Room* room = player->GetLocation();

        // The key opens the sanctuary gate from the village.
        if (room->GetName() == "Valdren Village" && itemName == "rusty key")
        {
            Exit* sanctuaryGate = room->GetExit(Direction::East);
            if (sanctuaryGate && sanctuaryGate->IsLocked())
            {
                sanctuaryGate->Unlock();
                std::cout << "You use the rusty key on the old gate.\n";
                std::cout << "The lock clicks open. The path to the sanctuary is clear.\n";
                SpendTime(2);
                return;
            }
        }

        // The lantern reveals the hidden stone in the Cave.
        if (room->GetName() == "Echoing Cave" && itemName == "lantern")
        {
            if (room->FindEntity("stone") == nullptr)
            {
                std::cout << "You illuminate the cave, revealing your surroundings.\n";
                std::cout << "A majestic silhouette reveals itself right in front of you.\n";
                std::cout << "The GUARDIAN of Valdren reveals himself to you\n";
            }
            else
            {
                std::cout << "The statue has already revealed its secret.\n";
            }
            SpendTime(2);
            return;
        }

        if (room->GetName() == "Old Sanctuary" && itemName == "lantern")
        {
            if (room->FindEntity("stone") == nullptr)
            {
                std::cout << "You bring the light closer to the statue of the king.\n";
                std::cout << "At the foot of the statue, a stone with a magical aura is revealed.\n";
                std::cout << "The King's Stone reveals itself to you.\n";
                CreateItemInRoom(*room, "stone", "A small glowing stone, hidden inside the king's statue.");
            }
            else
            {
                std::cout << "The statue has already revealed its secret.\n";
            }
            SpendTime(2);
            return;
        }

        // The stone opens the magical barrier on the bridge.
        if (room->GetName() == "Stone Bridge" && itemName == "stone")
        {
            Exit* towerExit = room->GetExit(Direction::North);
            if (towerExit && towerExit->IsLocked())
            {
                towerExit->Unlock();
                std::cout << "You use the glowing stone against the magical barrier.\n";
                std::cout << "The barrier fades. The road to the tower is open.\n";
                SpendTime(2);
                return;
            }
        }

        // The stone opens the final gate.
        if (room->GetName() == "The Tower" && itemName == "stone")
        {
            Exit* gate = room->GetExit(Direction::Up);
            if (gate && gate->IsLocked())
            {
                gate->Unlock();
                std::cout << "You place the glowing stone into the tower's ancient mechanism.\n";
                std::cout << "The Dawn Gate opens. Escape before midnight!\n";
                SpendTime(2);
                return;
            }
        }

        if (itemName == "letter") {
            std::cout << "You unfold the letter and read it carefully.\n";
            std::cout << "If you are reading this, the curse has already begun.\n";
            std::cout << "The Dawn Gate is the only way out.\n";
            std::cout << "Find the old guardian.He knows how to open it.\n";
            std::cout << "Do not trust the tower keeper.\n";
        }

        std::cout << "You cannot find a useful way to use the " << itemName << " here.\n";
        SpendTime(TimeCost::Use);
    }
    else if (command.verb == "talk")
    {
        for (const std::unique_ptr<NPC>& npc : npcs)
        {
            if (npc->GetLocation() == player->GetLocation() && npc->GetName() == command.target)
            {
                std::cout << npc->Talk() << "\n";
                SpendTime(TimeCost::Talk);
                return;
            }
        }
        std::cout << "There is nobody here with that name.\n";
    }
    else
    {
        std::cout << "I don't understand that command. Type 'help' to see the available commands.\n";
    }
}

void World::Run()
{
    PrintCurrentRoom();

    std::string rawInput;
    while (isRunning)
    {
        for (const std::unique_ptr<NPC>& npc : npcs)
            npc->Update();

        std::cout << "\n> ";
        if (!std::getline(std::cin, rawInput))
            break;

        Command command = CommandReader::Reader(rawInput);
        ExecuteCommand(command);
    }
}

bool World::IsRunning() const
{
    return isRunning;
}

void World::Stop()
{
    isRunning = false;
}

Player* World::GetPlayer() const
{
    return player.get();
}
