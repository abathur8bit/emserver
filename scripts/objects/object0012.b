'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "This is an army hand shovel."
 return 

onGet:
 pickupItem ov.id
' pv.RoomNum = 30
' print "Got it. You have been teleported to another room."
 return
