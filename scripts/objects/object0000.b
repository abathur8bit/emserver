'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
    print "This is a sturdy looking oak desk. There is a thick layer of dust"
    print "that covers it. There are two drawers in the desk. ";
    if ov.drawerOpen = 1 then goto drawerIsOpen

    print "One of the drawers is ajar."
    return 

drawerIsOpen:
    print "One of the drawers is open."
    return



onOpen:
    if pv.quest_drawerOpen = 1 then goto alreadyOpen
    'if ov.drawerOpen = 1 then goto alreadyOpen
    print "You tug one of the drawers open and find something inside!"
    pv.quest_drawerOpen = 1
    setObject found,17,2
    if found = 0 then goto setobjecterror
    ov.invisible = 0
    return

alreadyOpen:
    print "You try the other drawer, but it won't budge."
    return



onGet:
    print "It's way too heavy to carry!"
    return



onInit:
    ov.invisible = 0
    ov.drawerOpen = 0
    return
