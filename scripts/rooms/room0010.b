'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 3
 pv.Ypos = 4
 gosub onGlance
 return


briefDescription:
 print "SLOPPING PASSAGE"
 return


fullDescription:
 print "The floor slopes to the west. The air current from the west is strong here."
 print "You can see a crack in the wall here, and this must be where the air flow is"
 print "coming from."
 return

moveWest:
 pv.RoomNum = 9
 gosub onLeave
 return 

moveEast:
 pv.RoomNum = 11
 gosub onLeave
 return

showExits:
 print "Exits: West, East"
 return

onInit:
 return
