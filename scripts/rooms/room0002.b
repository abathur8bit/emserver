'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 1
 pv.Ypos = 2
 gosub onGlance
 return


briefDescription:
 print "WEST END OF PASSAGE"
 return

fullDescription:
 print "Looking both west and east, the maze looks long and loanly."
 return


moveEast:
 pv.RoomNum = 3
 gosub onLeave
 return 

moveWest:
 pv.RoomNum = 1
 gosub onLeave
 return

showExits:
 print "Exits: West, East"
 return

onInit:
 return
