# EMServer
**Easye MUD Server** is a Multi User Dungeon with a focus on easy system administration. It utilizes a BASIC interpreter for scripts, and as much logic will be handled in scripts. With a focus on using scripts, if there is an error in one room script, it won't effect players in a different room.

Scripts are in the `scripts` folder and have an extension of `.b`. 

# Status

Early prototyping stage. Current version is a bit of a proof of concept. You can currently:

- Build and run the server using **cmake**
- Connect to it on port **4001**. Multiple people can connect. Your user is saved when you type `quit` in the game. 
- Type `help` in the game to see commands.
- Type `showfunctions` to see BASIC functions that scripts can use.
- Wander around a map. 
- When other players enter the same room as you, you will see them, and they will see you.
- Interact with the vampire by typing `talk vampire` from anywhere in the game. The dialog isn't finished, so the script may abort with an error. 

# Building and Running

## Prerequisites


You will need to have [SSObjects](https://github.com/abathur8bit/ssobjects) built and installed.

## Build
If you are using [CLion](https://www.jetbrains.com/clion/) just open the `CMakeLists.txt` file as a project.

Command line on **linux**:

	$ cmake CMakeLists.txt
	$ make

Note that there may be a lot of warnings. This was originally created in in 2001, and compilers were a little more lax back then. 

## Running
Server runs on port **4001**.

	$ ./emserver

## Sample Session

```
$ nc -c localhost 4001
Welcome
By Lee Patterson

Login: lee
Password (not hidden): lee
Welcome back. You have logged in 24 time(s) before.
User id  : 1001
Handle   : Lee
Username : lee
Password : lee

SLOPPING PASSAGE
Exits: West, East

> w
ROOM 9
TREASURE ROOM
Exits: East
Items: desk
num players 1

> open desk
You tug one of the drawers open and find something inside!

> l
TREASURE ROOM
This is the treasure room. There are large stacks of gold coins
and gems beyong imagination!
Exits: East
Items: desk, candle

> talk vampire 

What do you want to say?
0) Nothing
1) Hello, what big teeth you have
2) You need a little light
3) Are you going to eat me?
? 3
He says, 'Actually, I don't eat people, I suck their blood.'
What do you want to say?
0) Nothing
1) Are you going to suck my blood?
2) Would you like to suck my blood?
? 1
A smirk plays accross his face. He says, 'I may if I choose so.' 

What do you want to say?
0) Nothing
1) Hello, what big teeth you have
2) You need a little light
3) Are you going to eat me?
? 3
He says, 'Actually, I don't eat people, I suck their blood.'
What do you want to say?
0) Nothing
1) Are you going to suck my blood?
2) Would you like to suck my blood?
? 0

> 
```


