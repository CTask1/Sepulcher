//CTask
#include"..\include\Choice.h"
#include"..\include\EventsS.h"
#include"..\include\Events.h"
#include"..\include\Trader.h"
#include"..\include\Player.h"
#include"..\include\Enemy.h"
#include"..\include\util.h"

void EventsS::oldChest() { // Old Chest
    type("You find an old chest. Let's see what's inside...\n");
    int itemType = randint(1, 2);
    if (itemType == 1)
        player.initArmor(Item::TYPE::ARM_IRON, Item::Source::FIND, (player.Race == Player::HUMAN ? 2 : 1));
    else
        player.initWeapon(Item::TYPE::WPN_MAGIC, Item::Source::FIND, (player.Race == Player::ELF ? 2 : 1));
}

void EventsS::hiddenArmory() { // Hidden Armory
    type("You find a hidden armory. Let's see what equipment is available...\n");
    int itemType = randint(1, 2);
    if (itemType == 1)
        player.initArmor(Item::TYPE::ARM_STEEL, Item::Source::FIND, (player.Race == Player::HUMAN ? 2 : 1));
    else
        player.initWeapon(Item::TYPE::WPN_GREAT, Item::Source::FIND, (player.Class == Player::FIGHTER ? 2 : (player.Class == Player::WIZARD ? -1 : 1)));
}

void EventsS::friendlyTraveler() { // Friendly Traveler
    type("You encounter a friendly traveler. They share some advice and give you a small gift.\n");
    {
        int giftType = randint(1, 3);
        if (giftType == 1) {
            type("The traveler gives you a health potion!\n");
            player.resources.addResource("Health Potion");
        }
        else {
            uint16_t expGain = (randint(1, 100) == 1) ? randint(25, player.level * 25) : randint(5, player.level * 10);
            type("The traveler shares valuable knowledge. You gained ", expGain, " experience points!\n");
            player.addExp(expGain);
        }
    }
}

void EventsS::travelingTrader() { // Traveling Trader
    type("You encounter a traveling trader!\nWould you like to browse their wares?\n");
    Choice tradeChoice;
    do tradeChoice = input("Enter choice (1. Yes / 2. No): ");
    while (!tradeChoice.isChoice(true, "yes", 1, "no", 2));

    if (tradeChoice.isChoice("yes", 1)) {
        Trader trader;
        player.resources.displayResources();
        trader.displayWares();
        while (true) {
            type (
                "\nWhat would you like to do?"
                "\n1. Trade"
                "\n2. Leave\n"
            );
            Choice choice;
            do choice = input("Enter choice: ");
            while (!choice.isChoice(true, "trade", 1, "leave", 2));

            if (choice.isChoice("trade", 1)) {
                player.resources.displayResources();
                trader.displayWares();
                std::string buyChoice = "";
                do {
                    buyChoice = capitalize(input("\nEnter the resource you want to trade for (or enter q to go back): "));
                    if (buyChoice == "Q")
                        break;
                    if (trader.resources[buyChoice] < 1)
                        type("The trader doesn't have that resource!\n");
                } while (trader.resources[buyChoice] < 1);
                if (buyChoice == "Q")
                    continue;

                bool noResources = true;
                int amountBuy = 0;
                do {
                    std::string strAmount = input("How many do you want? ");
                    amountBuy = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
                    noResources = (amountBuy == -1 || trader.resources[buyChoice] < amountBuy);
                    if (noResources)
                        type("The trader doesn't have that many resources!\n");
                } while (noResources);
                
                std::string sellChoice = "";
                do {
                    sellChoice = capitalize(input("Enter the resource you want to trade (or enter q to go back): "));
                    if (sellChoice == "Q")
                        break;
                    if (player.resources[sellChoice] < 1)
                        type("You don't have that resource!\n");
                    if (player.special.name == sellChoice) {
                        type("You can't trade your special item!\n");
                        sellChoice = "";
                    }
                } while (player.resources[sellChoice] < 1);
                if (sellChoice == "Q")
                    continue;

                int amountSell = 0;
                do {
                    std::string strAmount = input("How many do you want to trade? ");
                    amountSell = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
                    noResources = (amountSell == -1 || player.resources[sellChoice] < amountSell);
                    if (noResources)
                        type("You don't have that many resources!\n");
                } while (noResources);

                if (trader.resources.prices[buyChoice] * amountBuy != trader.resources.prices[sellChoice] * amountSell) {
                    type("\nThis trade is unbalanced. Do you want to do it anyway?\n");
                    Choice confirmChoice;
                    do confirmChoice = input("Enter choice (1. Yes / 2. No): ");
                    while (!confirmChoice.isChoice(true, "yes", 1, "no", 2));

                    if (confirmChoice.isChoice("no", 2))
                        continue;
                }

                type("\nYou got ", amountBuy, " ", buyChoice, " from the trader for ", amountSell, " ", sellChoice, ".\n");
                trader.resources[sellChoice] += ui16(amountSell);
                trader.resources[buyChoice] -= ui16(amountBuy);
                player.resources[sellChoice] -= ui16(amountSell);
                player.resources[buyChoice] += ui16(amountBuy);
                continue;
            } else
                type("You leave the trader.\n");
            break;
        }
    } else
        type("You pass by the trader.\n");
}