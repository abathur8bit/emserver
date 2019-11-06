'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
    'print "This is object number "; ov.id
    print "This is an aerosol can filled with some sort of liquid."
    print "The label says it's bat repelant."
    return 
 

onSpray:
    print "The can makes a hissing sound and a mist is sent forth."
    if pv.RoomNum = 12 then goto killBats
    print "And it smells terrible!"
    return
killBats:
    if rv.batsKilled = 1 then print "And it smells terrible!"
    if rv.batsKilled = 0 then print "The bats drop like flies!" : rv.batsKilled = 1
    return

