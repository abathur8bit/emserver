'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

briefDescription:
 print "HALL WITH LARGE DOOR"
 return

fullDescription:
 if rv.doorLocked = 1 then print "An impressive large locked door."
 if rv.doorLocked = 0 then print "An impressive large door."
 return

moveWest:
 pv.RoomNum = pv.RoomNum - 1
 gosub onLeave
 return

moveEast:
 pv.RoomNum = pv.RoomNum + 1
 gosub onLeave
 return

moveSouth:
 pv.RoomNum = pv.RoomNum + 8
 gosub onLeave
 return

showExits:
 if rv.doorLocked = 1 then print "W,E"
 if rv.doorLocked = 0 then print "S,E"
 return

onInit:
 rv.doorLocked = 1
 return
