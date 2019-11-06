'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "A small battery powered vacuum that fits in a shirt pocket."
 return 



onUse:
    checkInventory e,ov.id
    if e = 0 then print "You are not holding the vacuum." : goto theend
    checkInventory e,11
    if e = 0 then print "You try the switch, but nothing happens. No batteries I guess." : goto theend    
    if pv.RoomNum = 52 then goto vacuumGhosts
makeNoise:
    print "The vacuum makes a loud sound and there is suction at the open end."
    print "You turn it off again to conserve battery power."
    return
vacuumGhosts:
    if rv.ghostsKilled = 1 then goto makeNoise
    rv.ghostsKilled = 1
    print "Whooooosh! Vacuumed the ghosts up!"
    return



theend:
    return
