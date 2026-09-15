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
        default:               opposite = Direction::North; break;
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
    Room* entrance = CreateRoom("Entrada", "Una entrada polvorienta con una puerta de madera al norte.");
    Room* salaCentral = CreateRoom("Sala Central", "Una sala amplia con pasillos en varias direcciones.");
    Room* biblioteca = CreateRoom("Biblioteca", "Estanterias repletas de libros antiguos.");
    Room* sotano = CreateRoom("Sotano", "Un sotano humedo y oscuro. Hace frio aqui.");
    Room* jardin = CreateRoom("Jardin", "Un jardin descuidado, con hierba hasta la rodilla.");

    ConnectRooms(entrance, salaCentral, Direction::North);
    ConnectRooms(salaCentral, biblioteca, Direction::East);
    ConnectRooms(salaCentral, jardin, Direction::West);

    exits.push_back(std::make_unique<Exit>(Direction::South, salaCentral, sotano, true, "llave"));
    salaCentral->AddExit(exits.back().get());
    exits.push_back(std::make_unique<Exit>(Direction::North, sotano, salaCentral));
    sotano->AddExit(exits.back().get());

    CreateItemInRoom(*entrance, "llave", "Una llave oxidada.");
    CreateItemInRoom(*biblioteca, "libro", "Un libro con las tapas gastadas.");
    CreateItemInRoom(*jardin, "mochila", "Una mochila de tela resistente.", true);

    NPC* bibliotecario = CreateNPC("bibliotecario", "Un anciano rodeado de libros.", biblioteca);
    bibliotecario->AddDialogueLine("Bibliotecario: Cuidado con el sotano, la puerta esta cerrada con llave.");
    bibliotecario->AddDialogueLine("Bibliotecario: Dicen que la llave anda por la entrada de la casa.");

    targetRoom = sotano;

    player = std::make_unique<Player>("jugador", entrance);

    isRunning = true;

    std::cout << "=== ZORK ===\n";
    std::cout << "Moverte, coger objetos, dejarlos, guardarlos y hablar con NPC consume tiempo.\n";
    std::cout << "Comandos: ir <direccion>, coger <item>, dejar <item>, meter <item> en <contenedor>,\n";
    std::cout << "          hablar <npc>, inventario, mirar, ayuda, salir\n";
}

void World::PrintCurrentRoom() const
{
    std::cout << "\n[Hora: " << clock.GetFormattedTime()
        << " | Quedan " << clock.GetRemainingMinutes() << " min para medianoche]\n";
    std::cout << "\n" << player->GetLocation()->GetFullDescription() << "\n";
}

void World::PrintHelp() const
{
    std::cout << "Comandos disponibles:\n"
        << "  ir <direccion>            - norte, sur, este, oeste, arriba, abajo\n"
        << "  coger <item>              - coge un objeto del suelo\n"
        << "  dejar <item>              - suelta un objeto del inventario\n"
        << "  meter <item> en <cont>    - mete un objeto dentro de otro\n"
        << "  hablar <npc>              - habla con un personaje\n"
        << "  inventario                - muestra lo que llevas\n"
        << "  mirar                     - vuelve a describir la sala\n"
        << "  salir                     - termina la partida\n";
}

void World::SpendTime(int minutes)
{
    clock.Advance(minutes);
    std::cout << "(Te ha costado " << minutes << " min. Son las " << clock.GetFormattedTime() << ")\n";
    CheckEndConditions();
}

void World::CheckEndConditions()
{
    if (!isRunning)
    {
        return;
    }

    if (player->GetLocation() == targetRoom)
    {
        std::cout << "\nHas llegado al " << targetRoom->GetName()
            << " a las " << clock.GetFormattedTime() << ". ¡Lo has conseguido!\n";
        Stop();
        return;
    }

    if (clock.HasExpired())
    {
        std::cout << "\nEl reloj marca medianoche... no has llegado a tiempo.\n";
        Stop();
    }

}

void World::ExecuteCommand(const Command& command)
{
    if (command.verb.empty())
    {
        std::cout << "No te he entendido.\n";
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
    else if (command.verb == "inventary")
    {
        player->ListInventory();
    }
    else if (command.verb == "exit")
    {
        std::cout << "Hasta la proxima.\n";
        Stop();
    }
    else if (command.verb == "go" || command.verb == "move")
    {
        Direction direction;
        if (!Exit::TryReadDirection(command.target, direction))
        {
            std::cout << "Direccion no reconocida.\n";
            return;
        }

        Room* currentRoom = player->GetLocation();
        Exit* exit = currentRoom->GetExit(direction);
        if (exit == nullptr)
        {
            std::cout << "No puedes ir en esa direccion.\n";
            return;
        }

        if (exit->IsLocked())
        {
            const std::string& keyName = exit->GetRequiredKeyName();
            if (player->FindEntity(keyName) == nullptr)
            {
                std::cout << "Esa salida esta cerrada. Necesitas: " << keyName << "\n";
                return;
            }
            exit->Unlock();
            std::cout << "Usas " << keyName << " para abrir el paso.\n";
        }

        player->MoveTo(exit->GetDestination());
        if (isRunning)
        {
            PrintCurrentRoom();
        }
    }
    else if (command.verb == "pickup")
    {
        if (player->PickUp(command.target))
        {
            SpendTime(TimeCost::PickUp);
        }
    }
    else if (command.verb == "drop")
    {
        if (player->Drop(command.target))
        {
            SpendTime(TimeCost::Drop);
        }
    }
    else if (command.verb == "put")
    {
        if (player->PutInside(command.target, command.targetTwo))
        {
            SpendTime(TimeCost::PutInside);
        }
    }
    else if (command.verb == "talk")
    {
        bool found = false;
        for (const std::unique_ptr<NPC>& npc : npcs)
        {
            if (npc->GetLocation() == player->GetLocation() && npc->GetName() == command.target)
            {
                std::cout << npc->Talk() << "\n";
                found = true;
                SpendTime(TimeCost::Talk);
                break;
            }
        }
        if (!found)
        {
            std::cout << "Aqui no hay nadie con ese nombre.\n";
        }
    }
    else
    {
        std::cout << "No entiendo ese comando. Escribe \"ayuda\" para ver la lista.\n";
    }
}

void World::Run()
{
    PrintCurrentRoom();

    std::string rawInput;
    while (isRunning)
    {
        for (const std::unique_ptr<NPC>& npc : npcs)
        {
            npc->Update();
        }

        std::cout << "\n> ";
        if (!std::getline(std::cin, rawInput))
        {
            break;
        }

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
