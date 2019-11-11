'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 3
 pv.Ypos = 5
 gosub onGlance
 return


briefDescription:
 print "DUSTY CORNER"
 return


fullDescription:
 print "Off to the west, the passage dead ends. You can feel a slight breeze from the west."
 return

moveWest:
 pv.RoomNum = 10
 gosub onLeave
 return 

moveNorth:
 pv.RoomNum = 7
 gosub onLeave
 return

showExits:
 print "Exits: North, West"
 return

onInit:
 return
