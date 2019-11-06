'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "This is a small one man row boat. It looks about ready to fall"
 print "appart."
 return 

onGet:
 print "You can't carry a boat!"
 return 
