#include "GameClock.h"
#include <sstream>
#include <iomanip>

GameClock::GameClock(int deadlineMinutes)
    : currentMinutes(1200)
    , deadlineMinutes(deadlineMinutes)
{
}

void GameClock::Advance(int minutes) 
{
    currentMinutes += minutes;
}

bool GameClock::HasExpired() const 
{
    return currentMinutes >= deadlineMinutes;
}

int GameClock::GetRemainingMinutes() const 
{
    int remaining = deadlineMinutes - currentMinutes;
    return remaining > 0 ? remaining : 0;
}

std::string GameClock::GetHMTime() const 
{
    int hours = currentMinutes / 60;
    int minutes = currentMinutes % 60;

    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes;
    return out.str();
}
