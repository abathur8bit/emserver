'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "This is a bag of coins. Quite a few coins by the size of it."
 return 



onGet:
	pickupItem ov.id
	print "Got it."
	pv.score = pv.score + 1
	return



itemDropped:
	pv.score = pv.score - 1
	return
