'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

briefDescription:
 print "COBWEBBY ROOM"
 return

moveEast:
 pv.RoomNum = pv.RoomNum + 1
 gosub onLeave
 return

moveNorth:
 pv.RoomNum = pv.RoomNum - 8
 gosub onLeave
 return

moveSouth:
 pv.RoomNum = pv.RoomNum + 8
 gosub onLeave
 return

showExits:
 print "N,S,E"
 return
