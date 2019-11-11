'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 2
 pv.Ypos = 5
 gosub onGlance
 return


briefDescription:
 print "SHORT CORRIDOR"
 return


fullDescription:
 print "Short north south corridor. You can feel a breeze coming from the west."
 return

moveNorth:
 pv.RoomNum = 5
 gosub onLeave
 return 

moveSouth:
 pv.RoomNum = 11
 gosub onLeave
 return

showExits:
 print "Exits: North, South"
 return

onInit:
 return
