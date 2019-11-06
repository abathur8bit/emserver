'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"



onInit:
    ov.invisible = 1
    return



onExamine:
    print "This is object number "; ov.id
    print "Small silver key."
    return 
