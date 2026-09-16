THE LAST BELL - Zork prototype

All the mechanics have been implemented, along with some optional ones. I also added a mechanic of my own design: there is a time limit to reach the end, and certain actions you take consume that time.

//IMPORTANT
On another note, I started the project on September 11th, but due to a Git issue I encountered, I had to create a new repository; consequently, the first commit had to be larger than the one that started the original project.
//

Start time: 20:00
Deadline: 00:00

Main story route:
The Clearing -> Whispering Forest -> Valdren Village -> Old Sanctuary -> Echoing Cave -> back to Old Sanctuary -> Stone Bridge -> The Tower -> Dawn Gate

Important USE interactions:
- In Valdren Village: USE RUSTY KEY opens the sanctuary gate.
- In Old Sanctuary: USE LANTERN reveals the hidden STONE.
- At Stone Bridge: USE KING'S STONE opens the magical barrier.
- In The Tower: USE KING'S STONE opens the Dawn Gate.

NPCs:
- crow (Whispering Forest)
- villager (Valdren Village)
- guardian (Echoing Cave)

Every movement, pickup, drop, put, talk and use action consumes time. LOOK, HELP and INVENTORY do not consume time.

Quick test commands:
go north
go north
take rusty key
take lantern
use rusty key
go east
go down
use lantern
talk guardian
go up
use lantern
take king's stone
go east
use king's stone
go north
use king's stone
go up

Expected result: victory at approximately 21:45.
