'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = "candle"

#include "scripts/objects/objectdefault.b"



onInit:
	ov.isLit = 0
	ov.invisible = 1
	return



onExamine:
	print "This is object number "; ov.id
	print "Bee's wax candle. The strong oder from the bees wax is pleasant."
	if ov.isLit = 1 then goto litDesc else print "It is not lit at the moment."
	return 

litDesc:
	' displays either "The candle is burning brightly, and is almost out." or  "The candle is burning brightly."  
	print "The candle is burning brightly";
	if ov.health < 2 then print ", but is almost out";
	print "."
	if ov.health > 0 then ov.health = ov.health - 1
	if ov.health = 0 then ov.islit = 0
	return



onLight:
	checkInventory e,9
	if e = 0 then goto nothingToLightWith
	checkInventory e,ov.id
	if e = 0 then goto dontHave
	checkInventory e,8
	if e = 0 then goto noCandleStick

	ov.isLit = 1
	pv.nightvision = pv.nightvision + 75
	print "The candle casts a bright light."
	return

donthave:
	print "You must have the item to light it."
	return

nothingToLightWith:
	print "You don't have anything to light it with."
	return

noCandleStick:
	print "It will burn your hands. You need something to put it in."
	return



onUnlight:
    checkInventory e,ov.id
    if e = 0 then goto dontHave
    pv.nightvision = pv.nightvision - 75
    print "The candle is out."
    return



pickupItem:
	print "Picking up item id ";ov.id
	pickupitem ov.id
	return 
    
