'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 1
 pv.Ypos = 4
 gosub onGlance
 return


briefDescription:
 print "EAST END OF PASSAGE"
 return

fullDescription:
 print "This is the eastern end of a long passage."
 print "Looking west the maze looks long and loanly."
 return


moveWest:
 pv.RoomNum = 3
 gosub onLeave
 return

moveEast:
 pv.RoomNum = 5
 gosub onLeave
 return 

showExits:
 print "Exits: West, East"
 return

onInit:
 return
