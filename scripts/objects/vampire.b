onExamine:
    print "He has very pointy teeth."
    return

onTalk:
    print ""
	print "What do you want to say?"
	print "0) Nothing"
	print "1) Hello, what big teeth you have"
	print "2) You need a little light"
	print "3) Are you going to eat me?"
	input "? ";say
	if say=1 then goto teeth
	if say=2 then goto light
	if say=3 then goto eat
	if say=0 then goto theend
	print "Select from the list please." : goto onTalk
	return

teeth:
	print "He says, 'All the better to eat you with.'"
	return

light:
	if pv.neededlight >0 then goto crazylight
	pv.neededlight = 1
	print "He tilts his head and looks at you funny."
	return
crazylight:
	pv.neededlight =pv.neededlight+1
	if pv.neededlight < 3 then print "He says, 'You really are nuts.'" else print "He asks, 'You got a death wish or something?' "
	return

eat:
	print "He says, 'Actually, I don't eat people, I suck their blood.'"
eat1:
	print "What do you want to say?"
	print "0) Nothing"
	print "1) Are you going to suck my blood?"
	print "2) Would you like to suck my blood?"
	input "? ";say
	if say=0 then goto theend
	if say=1 then print "A smirk plays accross his face. He says, 'I may if I choose so.' " : goto ontalk
	if say=2 then goto showring
	print "Please choose from the list." : goto eat1
	return

showring:
	print "He smiles at you, and says 'No, but thank you for the offer. This may interest you."
	setobject 2,2
	ov.invisible = 0
	print "He pulls off something from his finger and tosses it on the floor in"
	print "front of you."
	return

theend:
	return
