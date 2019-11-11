' (be sure to put a blank line or a comment as the first line of a script do to a bug)

msg:
	' Message Processor will look at the action verb and gosub the appropreate subroutine. 
	' print "msgProcessor entering"
	processedMsg = 0
	if pv.Verb = 1 then gosub onMove : processedMsg = 1
	if pv.Verb = 2 then gosub onLook : processedMsg = 1
	' print "msgProcessor returning"
	return



onScore:
	print "You have picked up a total of ";pv.score;
	print " of a possible 7 treasures."
	if pv.score > 6 then goto gotAllTreasure
	return

gotalltreasure:
	if pv.RoomNum = 49 then playerWins
	print "You must return to the front porch to finish the game."
	return

playerWins:
	print "Congratulations! You won!"
	return

onEnter:
	'print "ROOM "; rv.id
	gosub onGlance

    countRoomItems objects,players,pv.RoomNum
    'print "num players ";players
    if players > 1 then gosub makeannouncement
	return
makeannouncement:
    m$ = pv.name$
    m$ = "arrived"
    announcePlayer 1,m$
    return

onLeave:
    countRoomItems objects,players,pv.RoomNum
    if players = 1 then announcePlayer 0
	return

onNorth:
	gosub moveNorth
	return
 
onSouth:
	gosub moveSouth
	return

onWest:
	gosub moveWest
	return
 
onEast:
	gosub moveEast
	return

onUp:
	gosub moveUp
	return
 
onDown:
	gosub moveDown
	return

onNorthWest:
	gosub moveNorthWest
	return

onNorthEast:
	gosub moveNorthEast
	return

onSouthEast:
	gosub moveSouthEast
	return

onSouthWest:
	gosub moveSouthWest
	return

moveNorth:
moveSouth:
moveWest:
moveEast:
moveNorthEast:
moveNorthWest:
moveSouthEast:
moveSouthWest:
moveUp:
moveDown:
	print "You can't move in that direction."
	return

onGlance:
	goto look

look:
	if rv.darkness = 0 then goto showall
	if pv.nightvision > rv.darkness then goto showall
	if pv.nightvision = rv.darkness then goto showsome
	print "It's too dark to see anything!"
	return
showall:
	gosub briefDescription
	if looking = 0 then goto showall1
	gosub fullDescription
'	print ""
showall1:
    print "Exits: ";
	gosub showExits
'	print ""
	gosub showRoomItems
'	print "---"
	return
showsome:
	gosub briefDescription
	gosub showExits
	print ""
	print "It's too dark to make out anything else."
	print "---"
	return

onLook:
	looking = 1
	goto look


showRoomItems:

  countRoomItems objects,players,pv.RoomNum
  'count in players includes the current player
  'print "Room item count ";objects; " players count ";players
  if objects > 0 then gosub showItemsInRoom
  if players > 1 then gosub showPlayersInRoom
  return
  
showItemsInRoom:
  print "Items: ";
  showItems
  print ""
  return

showPlayersInRoom:
  print "Players: ";
  showPlayers
  print ""
  return;

cantLook:
	print "Nothing to look at. Try examining things."
	return

onAnnounceArrive:
    print announceName$;" just entered the room."
    return

onAnnounceLeave:
    print announceName$;" just left the room."
    return

onInit:
    return

fullDescription:
    return
