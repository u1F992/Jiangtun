// #define DEBUG

#include "Nintendo.h"

#include "nxmc/nxmc.hpp"
#include "serial/logger.hpp"
#include "serial/receive.hpp"
#include "gc.hpp"
#include "utils.hpp"

CGamecubeConsole console(5);
Gamecube_Data_t data = defaultGamecubeData;

SerialLogger logger;
PacketHandler handler(
    Receive,
    [](Packet &packet, Logger &logger)
    {
        ToReport(packet, data.report);
#ifdef DEBUG
        DebugPrint(logger, packet, data);
#endif
    },
    [](const Packet packet, Logger &logger)
    {
        if (!console.write(data))
        {
            WarningNotConnected(logger);
            return;
        }
    },
    logger);

void setup()
{
    Initialize(console, data);

    Serial.setTimeout(100);
    Serial.begin(9600);
}

void loop()
{
    handler.Loop();
}
