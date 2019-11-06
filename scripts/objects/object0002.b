'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = ""

#include "scripts/objects/objectdefault.b"

onInit:
	ov.invisible = 1
	return


onExamine:
 print "This is object number "; ov.id
 print "Diamond ring."
 return 


checkDropOkay:
	ov.cursedCount=0
	if ov.cursedCount = 0 then goto curseLifted
	dropOkay = 0
	ov.cursedCount = ov.cursedCount - 1
	print "This cursed ring won't come off!"
	return
 
curseLifted:
	return


onGet:
	' you have to try dropping the item 5 times
	' ov.cursedCount = 4

	pickupItem ov.id
	print "Got it."
	pv.score = pv.score + 1
	return



itemDropped:
	pv.score = pv.score - 1
	return



onOpen:
	checkInventory exist,ov.id
	if exist = 0 then goto what
	if ov.invisible = 0 then goto openRing
what:
	print "What?"
	return
openRing:
	if rv.id = 30 then goto inroom
	print "When you open the ring, you hear a rumble in the other room."
	setRoom 30
	rv.barsClosed = 0
	return
inroom:
	print "You see part of the wall fall away."
	rv.barsClosed = 0
	return
