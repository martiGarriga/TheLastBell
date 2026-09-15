#pragma once

#include <string>

class GameClock {
private:
    int currentMinutes;
    int deadlineMinutes;

public:
    explicit GameClock(int deadlineMinutes = 1440);

    void Advance(int minutes);
    bool HasExpired() const;
    int GetRemainingMinutes() const;

    // Devuelve la hora formateada como "HH:MM"
    std::string GetFormattedTime() const;
};

