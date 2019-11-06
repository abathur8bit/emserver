onExamine:
    if pv.roomnum <> 43 then goto genericWall
    print "The wall is very cracked and looks about to fall over."
    print "Close inspection reveals there is another room behind it."
    return

genericWall:
    if pv.word$ = "wall" then print "Looks like a normal wall to me." else  print "Looks like a set of walls to me."
    return
