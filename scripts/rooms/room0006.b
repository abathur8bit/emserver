'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 2
 pv.Ypos = 1
 gosub onGlance
 return


briefDescription:
 print "ENTRANCE HALLWAY"
 return

fullDescription:
 print "This leads directly to the entrance of the maze."
 return

moveNorth:
 pv.RoomNum = 1
 gosub onLeave
 return 

moveSouth:
 pv.RoomNum = 8
 gosub onLeave
 return

showExits:
 print "Exits: North, South"
 return

onInit:
 return
