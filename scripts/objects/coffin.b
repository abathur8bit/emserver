onExamine:
    if pv.seenVamp = 1 then goto seenVampire
    print "It's a lavishly decorated coffin, and oh!"
    print "There is a vampire staring back up at you."
    print "'Hello,' he says."
    pv.seenVamp = 1
    return

seenVampire:
    print "He looks like he woke up from a long sleep."
    return
