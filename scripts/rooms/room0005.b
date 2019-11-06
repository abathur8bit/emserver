'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

onEnter:
 pv.Xpos = 1
 pv.Ypos = 5
 gosub onGlance
 return


briefDescription:
 print "EASTERN CORNER"
 return


fullDescription:
 print "This is the eastern corner of a long passage. Looking west, the"
 print "maze looks long and loanly."
 return

moveWest:
 pv.RoomNum = 4
 gosub onLeave
 return 

moveSouth:
 pv.RoomNum = 7
 gosub onLeave
 return

showExits:
 print "Exits: West, South"
 return

onInit:
 return
