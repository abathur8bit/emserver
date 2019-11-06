'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "This is an ancient scroll. You don't recognize the language it's"
 print "written in."
 return 



onGet:
	pickupItem ov.id
	print "Got it."
	pv.score = pv.score + 1
	return



itemDropped:
	pv.score = pv.score - 1
	return
