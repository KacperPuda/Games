# Ships
 Game written for the Basics of Programming course on IT in Gdansk University Of Technology.

EXPLANATIONS:

Details of the interface:
The commands are grouped into two groups: the description of the state of the game, the description of a players move in a given turn. E.g. a group [playerA] [playerA] specifies the commands given by A in a single turn. The players are doing the turns alternately. The commands inside a group should be executed sequentially, e.g. if a ship moves, shoots, and moves, then the shoot should be done with respect to the first move.


The group of state commands starts with [state] and the commands from this group are provided until another [state] is encountered. There are


Basic:

PRINT x
Print the state of the game. For x = 0 it is the basic version. The known fields should be printed as either: ‘+’, ‘x’, ‘ ’. ‘+’ means that the field is occupied by a ship. 'x' means that there is a destroyed fragment at the position. ' ' means that the position is empty. Also, print a line containing an information how many undestroyed parts of ships remain.

SET_FLEET P a1 a2 a3 a4
The number of ships of consecutive classes (CARRIER ... DESTROYER) for the player P are a1 ... a4. If the command is not specified the default numbers 1, 2, 3, 4 apply.

NEXT_PLAYER P
Sets an information that P should make move next.

Extended:

BOARD_SIZE y x

Set the board size to y times x

INIT_POSITION P y1 x1 y2 x2
The player P can place ships in rectangle given by (y1, x1) (left upper) and (y2, x2) (right lower). It means that the player can place ships in [y1, y2] in Y dimension and in [x1, x2] in X dimension - the edges are included.

REEF y x
There is a reef at position (y,x). It should be printed as #.

SHIP P y x D i C a1...al
There is a ship of player P at position (y,x) in the direction D(N/W/S/E) i-th of class C (CAR/BAT/CRU/DES). The state of the segments are a1 … al, where l is the length of the ship and 1 means that the part is not destroyed, 0 that it was destroyed. This command is unaffected by starting positions requirements. Keep in mind that there are no spaces between a1 and a2, …, al-1 and al.

EXTENDED_SHIPS
Turn the extended logic of ships. I.e. the ships are composed of the 2 sections, etc. Turning this on disables the default SHOOT operations by players.

SET_AI_PLAYER P
The player P becomes an A.I.

PRINT x

The printing with x = 1 should follows the advanced rules.

SRAND x
Set the seed of chosen random number generator to x. Please use this seed in your program to make the executions consistent.

SAVE
Save the commands that allows to recreate the state of the game. Please, save even the default information. The order of saving should be: board size; which players is next; initial positions of A, sizes of fleets of A, placements of ships of A (in order of types and ids), then the same information for B; Also, if present, reefs(in the order given in the input), extended_ships, autoshooting, if A.I. was set. Also, please save the seed of random number generator, but increased by 1, if given.

The group of player commands starts and ends with [playerX], where X is either A or B. Commands are give until another corresponding [playerX] is given. In the [player] group you have:

Basic Logic:

PLACE_SHIP y x D i C
Place the i-th ship of class C at (y,x) with direction equal to D.

SHOOT y x
Shoot at the field (y,x). Shooting can only start if all the ships were placed.

Extended Logic:

MOVE i C x
Move the i-th ship of class C x ([F]orward, [L]eft, [R]ight)

SHOOT i C y x
Let one of the shoots of i-th class ship be at (y,x).

SPY i y x
Send a spy plane from i-th carrier at the position (y,x).

PRINT x
Print the state of the game as seen by the player. That is, print the state as seen using radars and spy planes. The reefs are known every time. If a field is empty print ' '. If a field contains a visible part of ship print it. If a field is unknown print '?'. For x = 0 use basic printing. For x = 1 advanced one.

Advanced Logic:

SHOOT y x
Shoot at the field (y,x).

SPY y x
Send a spy plane at (y,x).