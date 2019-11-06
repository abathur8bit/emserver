'                   1         2         3         4         5         6         7         8         9
'          123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
onExamine:
    print "This is the largest Praying Matis you have ever seen. He "
    print "looks both wise and powerfull. You wouldn't want to mess"
    print "with him, but at the same time, he has a kind look in"
    print "his eyes."
    return

onTalk:
    if pv.hostTalkedTo=0 then goto firstMeeting
    if pv.hostTalkedTo>0 and pv.hostTalkedTo<2 then goto metBefore
    'met a lot
    print "Hello again "; pv.name$; ". Good of you to stop by. I'm "
    print "Quite busy right now however. Please return another time."
    return

firstMeeting:
    print "Hello there "; pv.name$; ". I'm glad we are finally able to meet. ";
    print "You are in a haunted house. Please watch for ghosts. Although they are ";
    print "quite harmless at the moment, they could turn hostle at any time."
    print "Well, it was nice talking with you. Good luck. Stop by again some time."
    pv.hostTalkedTo = pv.hostTalkedTo + 1
    return 
    
