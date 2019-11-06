' called when the player examines this object.
onExamine:
 print "I see nothing special."
 return

' Called when a person is looking at a room. 
' If the object is not visable, you don't print
' anything.
onLook:
 return

' called when the player picks the object up
' This is where you would check if the player 
' is able to carry this item, and if you need
' to modify stats on the player as a resuly
' of the player picking up the object.
onGet:
 print "Picking up item "; ov.id
 pickupItem ov.id
 return 


' Called whtn the player is dropping an item
' from his inventory
onDrop:
	checkInventory exists,ov.id
	if exists = 0 then goto itemNotInInventory
	dropOkay = 1
	gosub checkDropOkay
	if dropOkay = 1 then goto dropItemIsOkay
	return

dropItemIsOkay:
	print "Dropping item ";ov.id
	dropItem ov.id
	gosub itemDropped
	return

itemNotInInventory:
	print "You are not carrying that item."
	return 
 

' Override this if you need to check if you will allow a player
' to drop this object. For instance the player may not be able
' to drop a cursed item.

checkDropOkay:
	return



' this is called when the item has been dropped. Override
' this when you need to do something after the object is 
' dropped.

itemDropped:
	return
 
' Called when the object is first created.
' This is where you initialize and variables 
' you will use for this object. There is no
' player or room attached at this time, so 
' using print will result in no output.
onInit:
 return

setObjectError:
 print "Error in script [";errormsg$;"]"
 end
