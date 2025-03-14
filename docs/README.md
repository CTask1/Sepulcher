# NOTE
This game uses the C++20 standard. It can be compiled with MSVC using the following command: `cl.exe /std:c++20 /EHsc /nologo /FeRPG.exe src\RPG.cpp src\Player.cpp src\PlayerPrivate.cpp src\PlayerPublic.cpp src\Events.cpp src\EventsS.cpp src\EventsR.cpp src\EventsD.cpp`

# Welcome to the game!

Before you get started, there are a few things you should know:


## Races

When starting the game, you will be prompted to choose a race for your character.
Here is some information about each race:


### Elf

- Constitution: 3
- Advantage with magic weapons


### Human

- Constitution: 3
- Advantage with heavy armor


### Drakonian

- Constitution: 3
- Natural defense: 1, increasing every level
- Cannot wear normal armor
- Combat ability: Dragon's Breath (Burns the enemy)


## Classes

After you choose a race, you must choose a class.
The class you choose affects the difficulty of the game.


### Fighter

Fighter is a medium-difficulty class. It has one ability.

- Hitdie: 16
- Strength: 4
- Advantage with heavy weapons
- Ability: Second Wind (Heals the player)


### Rogue

Rogue is the easiest class, as it has no abilities to keep track of.

- Hitdie: 14
- Strength: 4
- Can surprise enemies in the first round of combat **unless wearing heavy armor**


### Wizard

Wizard is the hardest class.
It has many weapons and spells to keep track of, and the player must be smart about how they manage their mana points.

- Hitdie: 12
- Strength: 4
- Mana points: 15
- Cannot wear armor
- Disadvantage with non-magic weapons
- Magical Resilience: Wizards regain health equal to mana points used for spells.
- Spells
    - Fire Bolt
        - Only used in combat
        - Burns the enemy
        - Costs one mana point
    - Mirror Image
        - Only used in combat
        - Enemy hit chance is 0% for one round, then 50%, then normal
        - Costs two mana points
    - Mage Armor
        - Acts as armor for one day
        - Has the same defense as iron armor
        - Costs five mana points
    - Arcane Eye
        - Only used outside of combat
        - The user cannot be surprised or trapped for one day
        - Costs three mana points
    - Recovery
        - Given by the Staff of the Guardian
        - Heals the player
        - Costs two mana points
- Ability: Rituals
- Rituals (Only used outside of combat)
    - Mana Restoration
        - Requires an Arcane Focus
        - Recharges four mana points


## Items

There are currently 19 items you can find or craft in the game.
Here is a list of them:

- Armor (non-Wizards only)
    - Drakonian Armor (Drakonians only)
        - Crafted with six leather, two fiber
        - Low defense
    - Leather Armor (non-Drakonians only)
        - Crafted with six leather, two fiber
        - Low defense
    - Iron Armor (non-Drakonians only)
        - Found
        - Heavy armor
        - Medium Defense
    - Steel Armor (non-Drakonians only)
        - Found
        - Heavy armor
        - High Defense
- Weapons
    - Longsword
        - Crafted with two wood, two fiber, three iron
        - Medium attack
    - Greatsword
        - Found
        - Heavy weapon
        - Medium attack
    - Magic Sword
        - Found
        - Magic weapon
        - Medium attack
    - Crossbow
        - Dropped by Goblins
        - Medium attack
- Items
    - Arcane Focus (Wizards only)
        - Crafted with four crystals


### Other

There are many special items you can get while playing.
Here is a list of them:

- Amulets  
You can only have one amulet at a time, and amulets are equipped automatically when they are found.  
    - Amulet of the Warborn
        - Increases defense and strength
    - Amulet of the Guardian
        - Increases defense
    - Amulet of the Shadow
        - Gives a 10% chance for enemies to miss attacks
    - Amulet of Fury
        - Decreases defense but increases strength
    - Amulet of the Weeping Spirit
        - Decreases defense and strength
- Staves (Wizards only)  
Staves increase the strength of the player, and each has its own bonus effect.  
    - Staff of the Warborn
        - Increases mana points
        - Magic weapon
    - Staff of the Guardian
        - Gives the Recovery spell
        - Magic weapon
    - Staff of the Shadow
        - Gives a 100% chance to run from combat
        - Magic weapon
    - Staff of Fury
        - Increases strength further
        - Magic weapon
    - Staff of the Weeping Spirit
        - Decreases mana points
        - Magic weapon


## Events

There are 13 events that can occur each time the player explores.  
These events will not be explained. Play the game to see what they are!  
Events are split into three types. Here is a list of them:

- Safe  
Safe events happen 60% of the time.  
There is no risk of damage, and most of these events give the player something.  
    - Friendly Traveler
    - Old Chest
    - Hidden Armory
    - Peaceful Meadow
    - Hidden Garden
    - Traveling Trader
    - Nothing
- Risky  
Risky events happen 30% of the time.  
There is risk of combat and debuffs.  
    - Lost Traveler
    - Mountain Pass
    - Mysterious Cave
    - Strange Amulet
- Dangerous  
Dangerous events happen 10% of the time.  
Combat and/or damage is highly likely.  
    - Enemy Encounter
    - Poacher's Snare