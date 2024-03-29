'score      - same as stats
'stats      - Shows player attributes

main:
 print "No subroutine called"
 return

onShowPrompt:
    'if pv.admin = 1 then print "Room num: ";pv.roomnum
    print ""
    print "> ";
    return


onLogin:
	print "Welcome"
	print "By Lee Patterson"
	print ""

getlogin:
	input "Login: ";pv.username$
	loadUser found,pv.username$
	if found = 0 then goto newUser
	input "Password (not hidden): ";password$
	if password$ <> pv.password$ then print "Invalid username or password" : goto getlogin

userLoggedIn:
	' print "onLogin entering"
	gosub SetupLogin
	if pv.LoginCount > 0 then print "Welcome back. You have logged in ";pv.loginCount; " time(s) before." else gosub intro

    '---[DEBUG]---
    print "User id  : "; pv.id
    print "Handle   : "; pv.name$
    print "Username : "; pv.username$
    print "Password : "; pv.password$
    print ""
    '---[DEBUG]---

	pv.LoginCount = pv.LoginCount + 1

	' print "onLogin returning"

endLogin:
	return

newUser:
	input "Are you a new user [y/n]? "; yesno$
	if yesno$ = "y" then goto createNew
	if yesno$ = "Y" then goto createNew
	if yesno$ = "yes" then goto createNew
	if yesno$ = "YES" then goto createNew
	goto getLogin

createNew:
    print "Creating new user"
    ' for now a players handle is the same as the user name to make
    ' sure it is unique
    pv.name$ = pv.username$

	input "Please enter a password  : ";pv.password$

    'starting room number
	pv.RoomNum = 49
	pv.AreaNum = 2
	goto userLoggedIn



onLogout:
	input "Do you wish to leave the game [y/n]? ";yesno$
	if yesno$ <> "y" then print "Logout aborted" : goto logoutend
	print "Until next time..."
	logoutUser
logoutEnd:
	end



onScore:
	print "You have picked up a total of ";pv.score;
	print " of a possible 7 treasures."
	pv.bleh$ = "hello"
	print "hmm ";pv.bleh$
	if pv.score > 6 then goto gotAllTreasure
	return

gotAllTreasure:
	if pv.RoomNum = 49 then goto playerWins
	print "You must return to the front porch to finish the game."
	return

playerWins:
	print "Congratulations! You won!"
	print "This quest is now finished."
	return



onInventory:
 print "You are carrying:"
 showInventory
 return


onObjectNotFound:
 print "I don't see what you are talking about."
 return


onTeleport:
 if pv.Admin = 0 then goto notAuthorized
 input "Room number to jump to? "; room
 if room < 64 then goto doTeleport
 print "Room is out of range."
 goto onTeleport

doTeleport:
 pv.RoomNum = room
 return


onHelp:
 print "Known commands are ..."
 help
 print "-----"
 return

onShowFunctions:
 if pv.Admin = 0 then goto notAuthorized
 print "Known script funcions/commands:"
 showFunctions
 print ""
 'help
 return

onCmdNotUnderstood:
 print "I don't understand."
 return


onReload:
	print "1) Commands"
	print "2) Words"
	input "Reload what? ";module
	reload module
	print "Reloaded"
	return


onJump:
 print "You jump up and down on the spot. This is great exersize!"
 return

onRoom:
	print "You are currently standing in room ";pv.RoomNum;"."
	return

setupLogin:
 Payment = 10
 DefaultHealth = 100
 return


payDay:
 print "You have been paid ";Payment;" credits."
 pv.Credits  = pv.Credits + Payment
 return


intro:
 print "H A U N T E D   H O U S E"
 print ""
 print "Welcome to the Haunted House Demo, by Lee Patterson."
 print "The Haunted House is a small adventure game meant to demonstraight"
 print "some of the capabilities of the EasyeMud engine. The mud engine is written in"
 print "C++, and all the game content is written  in a script language similar to Basic."
 print "In fact, the text you are now reading was generated by the player.b script's"
 print "onLogin handler."
 print ""
 input "Press ENTER to continue...";a

 print "Scripts can be modified real time. That is to say, if you make a change to"
 print "a script, it will be reflected in the game right away, and you don't have to"
 print "restart the server. It's a great way to make fixes to rooms or quests."
 print ""
 print "The demo world has purposely been left very sparse, as other builders want to"
 print "create their vision of th world, so why would he want to get in the way?"
 print ""
 input "Press ENTER to continue...";a

 print "However, he wanted to highlight a couple things:"
 print "o Type 'help' to get a list of available commands."
 print "o Type 'look' to get a full description of a room."
 print "o The diamond ring is cursed. You have to try many times to drop it."
 print "o The bars in the Cellar can be dug out using the shovel; which can be found at "
 print "  the north end of the house."
 print "o You can talk with the vampire in the Cellar."
 print ""
 input "Press ENTER to start playing!";a

 return

notAuthorized:
 print "You are not authorized to perform that command."
 goto theend

theend:
 end
