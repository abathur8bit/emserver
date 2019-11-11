'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 1
 pv.Ypos = 3
 gosub onGlance
 return


briefDescription:
 print "LONG PASSAGE"
 return

fullDescription:
 print "Looking east, the maze looks long and loanly."
 return


moveEast:
 pv.RoomNum = 4
 gosub onLeave
 return 

moveWest:
 pv.RoomNum = 2
 gosub onLeave
 return

showExits:
 print "Exits: West, East"
 return

onInit:
 return
