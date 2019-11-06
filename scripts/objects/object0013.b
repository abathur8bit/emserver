'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onExamine:
 print "This is object number "; ov.id
 print "This is small axe. The blade is a little rusty, but looks "
 print "like it will still cut."
 return 



onSwing:
	checkInventory e,ov.id
	if e = 0 then goto dontHave	

	if pv.RoomNum = 43 then goto okaySwing
swish:
	print "It makes a SWISH sound."
	return

dontHave:
	print "What?"
	return

okaySwing:
	if rv.wallBroken = 1 then goto swish
	rv.wallBroken = 1
	print "With a mighty swing, and a loud crash, the false wall crashes down."
	return



theend:
	return

