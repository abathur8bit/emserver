'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = "candle"

' NOTES:
' When player picks this item up, it will set a timer to check if this item
' it lit, and if it is, then decrement the health.

#include "scripts/objects/objectdefault.b"

onExamine:
 print "Item id "; ov.id
 print "It is round and is made of bee's wax. The strong oder from the bees wax"
 print "is plesant."
 if ov.isLit = 1 then goto litDesc else print "It is not lit at the moment."
 return

onLook:
 visable = 1
 return

litDesc:
 ' Prints:
 '  "The candle is burning brightly, and is almost out." or
 '  "The candle is burning brightly."

 print "The candle is burning brightly";
 if ov.health < 2 then print ", but is almost out";
 print "."
 if ov.health > 0 then ov.health = ov.health - 1
 if ov.health = 0 then ov.islit = 0
 return

pickupItem:
 print "Picking up item id ";ov.id
 pickupitem ov.id
 return 

onInit:
 ov.isLit = 1
 ov.health = 5
 return

