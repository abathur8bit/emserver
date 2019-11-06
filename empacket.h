#ifndef EMPACKET_H
#define EMPACKET_H

#include <ssobjects/packetbuffer.h>

using namespace ssobjects;

/**
   \brief Contains all the definitions of packet types.
**/
class EMPacket : public PacketBuffer
{
  public:
    enum
    {
      pcTextCmd = pcUser+1,           ///< Text command. Client usually sends them to the server.
      pcPlayerPosition,               ///< Players x&y position in the map.
    };
};

#endif //EMPACKET_H
                              
