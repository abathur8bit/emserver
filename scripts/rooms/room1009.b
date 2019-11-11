'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
#include "scripts/rooms/roomdefaults.b"

'onEnter:
'pv.Xpos = 3
'pv.Ypos = 3
'gosub onGlance
'print "enter here"
'return


briefDescription:
 print "TREASURE ROOM"
 return


fullDescription:
 print "This is the treasure room. There are large stacks of gold coins"
 print "and gems beyong imagination!"
 return

moveEast:
 pv.RoomNum = 10
 gosub onLeave
 return 

showExits:
 print "Exits: East"
 return

onInit:
 return
