# M.T.E:O.E Hook
**MTEOEHook** is a open-source trainer and online multiplayer client for *Monster Trux Extreme: Offroad Edition*.                             
It allows you to use some cheats, customize controls and use a W.i.P online multiplayer so you can play with multiple friends.                     

It only works for the Nestlé/World-Wide version of the game.                    
M.T.E:O.E Hook running on Windows:                   
![mteoehook](https://github.com/BuilderDemo7/MTEOEHook/blob/master/mteoehook.png)

# Discord Server
New members are very welcome!                    
https://discord.gg/hVexf6SMye

# Online Multiplayer
The online multiplayer mod is still W.i.P, to enable it go to the configuration file **MTEOEHook.ini** 
and set **Multiplayer** to 1.                                   
     
# Before launching the multiplayer	 
1. Using any hex editor like HxD open **DataPC.wad** in your game directory. Go to the find tool and search for the text "numVehicles"                
2. Find some that contains "4" around it for example: "numVehicles|(S32)**4**"                                             		   
3. Replace that "4" by "8", 8 is the maximum vehicles in a race, if bigger than that the game will crash.                                       	
	
Make sure your friend is in the same track as yours and that he did not select the same monster truck as yours.                      
## Server
You can start a server by simple executing **MTEOEMPServer.exe**, it uses the same configuration file as the client (MTEOEHook.ini)
# Customizing Controls
This feature is pretty buggy and might not make your experience in game good until it's fixed.                           
Open **MTEOEHook.ini**, set *CustomInput* to 1, you will have the following controls to customize:

- AccelerateKey is the key for accelerating your truck *(Default: Enter, VK: 13)*
- BrakeKey is the key for going in reverse and braking *(Default: Right-Shift, VK: 161)*
- LeftKey is the key for turning your truck to the left *(Default: Left Arrow, VK: 37)*
- RightKey is the key for turning your truck to the right *(Default: Right Arrow, VK: 39)*