# BUILDING

## Windows

This game can be compiled in release mode with MSVC using the following command in the Developer Command Prompt for VS 2022: `cl.exe /std:c++20 /EHsc /O2 /DNDEBUG /MT /MP /GL /Zc:preprocessor /nologo /Iinclude\ /Febin\release\Sepulcher.exe /Fobin\release\ src\main.cpp src\uEvents.cpp src\uPlayer.cpp src\uOther.cpp`  
You can then run it with the command: `bin\release\Sepulcher.exe`  
Alternatively, you can compile it in VSCode in the Run and Debug tab with the `Build (Release)` option.

NOTE: This game should be compatible with every OS, but it has never been tested outside Windows.


# Welcome to the game!

Welcome to the world of Sepulcher, a text-based RPG where you can explore and build your strength through random events in which you fight, gather resources, craft and find items, and trade until you eventually reach your final destination: the Sepulcher.

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


### Revenant

- Constitution: 3
- Healing has half normal effect
- Use Necrotic Drain to heal and fill a blood meter up to three times per day
    - If this meter is not filled by the end of the day, becomes Ravenous
- Debuff: Ravenous (Reduces max health by 10% for one day)
- Combat ability: Shadowmeld (Guarantees enemy will miss their next attack)
- Resurgence (Resurrect at 50% health once)
- Restless (Cannot heal from sleeping)


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
- Strength: 5
- Mana points: 15
- Cannot wear armor
- Disadvantage with non-magic weapons
- Use mana points to cast spells
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
- Magical Resilience: Wizards regain health equal to mana points used for spells.
- Ability: Rituals
- Rituals (Only used outside of combat)
    - Mana Restoration
        - Requires an Arcane Focus
        - Recharges four mana points


## Items

There are currently 20 items you can find or craft in the game.  
Armor and weapons have a chance to come with prefixes and/or suffixes, which can affect the item in a few different ways.  
Here is a list of them:


### Armor (non-Wizards only)

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


#### Prefixes

- Rusted (-20% defense)
- Worn (-10% defense)
- Heavy (+10% defense)
- Enchanted (+20% defense)


#### Suffixes

- of Thorns (+10% damage reflection)
- of the Knight (+10% defense)
- of Fortitude (+20% defense)
- of Resilience (+30% defense)


### Weapons

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
- Staves (Wizards only) (Cannot have prefixes)  
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
    

#### Prefixes

- Cursed (-20% strength)
- Dull (-10% strength)
- Sharp (+10% strength)
- Legendary (+20% strength)


#### Suffixes

- of the Inferno (+1d4 fire damage)
- of Vengeance (+10% strength)
- of the Slayer (+20% strength)
- of the Executioner (+30% strength)


### General

- Health Potion
    - Found or crafted with 2 medicinal herbs
- Arcane Focus (Wizards only)
    - Crafted with four crystals




### Special

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
    - Small Stream
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
    - Sepulcher