'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 gosub onGlance
 return


briefDescription:
 print "ENTRANCE"
 return

fullDescription:
 print "This is the main entrance to the maze."
 return

moveNorth:
 pv.RoomNum = 8
 gosub onLeave
 return 


showExits:
 print "Exits: North"
 return

onInit:
 return
