'                1         2         3         4         5         6         7         8         9
'       123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

'name = "food"

onExamine:
 if pv.RoomNum = 0 then gosub examine : return
 print "You don't see that here."
 return 

examine:
 if pv.LookedAtFood = 1 then goto seenFood

 print "This is a scruptious looking morsel of food fit for a king."
 print "You have to wonder though, how it get here?"
 pv.LookedAtFood = 1
 return

seenFood:
 print "Wonder how this food got here?"
 return
 

onGet:
 print "You try picking up some, but it slips. You notice it leaves"
 print "a thin coating on you where you touched it."
 return
