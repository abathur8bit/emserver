'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

briefDescription:
 print "CRUMBLING WALL"
 return

moveNorth:
 pv.RoomNum = pv.RoomNum - 8
 gosub onLeave
 return

showExits:
 print "N"
 return
