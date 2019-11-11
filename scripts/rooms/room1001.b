'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 gosub onGlance
 return


briefDescription:
 print "WESTERN CORNER"
 return


fullDescription:
 print "This is the western corner of a long passage. Looking east, the"
 print "maze looks long and loanly."
 return

moveEast:
 pv.RoomNum = 2
 gosub onLeave
 return 

moveSouth:
 pv.RoomNum = 6
 gosub onLeave
 return

showExits:
 print "Exits: East, South"
 return

onInit:
 return
