'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = "key"

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is a nice shinny silver key."
 print "Item id "; ov.id
 return

pickupItem:
 print "Picking up item id "; ov.id
 pickupitem ov.id
 return 

