//CTask
#include"TextManager.h"
#include"Choice.h"
#include"EventsS.h"
#include"Events.h"
#include"Trader.h"
#include"Player.h"
#include"Enemy.h"
#include"util.h"

void EventsS::oldChest() {
    type(TM::get("events.safe.chest.message"));
    int itemType = randint(1, 2);
    if (itemType == 1)
        player.initArmor(Item::TYPE::ARM_IRON, Item::Source::FIND, (player.Race == Player::RACE::HUMAN ? 2 : 1));
    else
        player.initWeapon(Item::TYPE::WPN_MAGIC, Item::Source::FIND, (player.Race == Player::RACE::ELF ? 2 : 1));
}

void EventsS::hiddenArmory() {
    type(TM::get("events.safe.armory.message"));
    int itemType = randint(1, 2);
    if (itemType == 1)
        player.initArmor(Item::TYPE::ARM_STEEL, Item::Source::FIND, (player.Race == Player::RACE::HUMAN ? 2 : 1));
    else
        player.initWeapon(Item::TYPE::WPN_GREAT, Item::Source::FIND,
            (player.Class == Player::CLASS::FIGHTER ? 2 : (player.Class == Player::CLASS::WIZARD ? -1 : 1)));
}

void EventsS::friendlyTraveler() {
    type(TM::get("events.safe.traveler.message"));
    {
        int giftType = randint(1, 3);
        uint16_t expGain = 0;
        if (giftType == 1)
            player.resources.addResource("Health Potion");
        else {
            expGain = (randint(1, 100) == 1) ? randint(25, player.level * 25) : randint(5, player.level * 10);
            player.addExp(expGain);
        }
        type(TM::getForCondition("events.safe.traveler.gift", giftType == 1, {
            .replacements = {{"{exp}", std::to_string(expGain)}},
            .end = '\n'
        }));
    }
}

void EventsS::travelingTrader() {
    type(TM::get("events.safe.trader.choose_option"));
    int tradeChoice;
    do tradeChoice = Choice(input(prompt.data())).isChoice({"yes", "no"});
    while (tradeChoice == 0);

    if (tradeChoice == 2) {
        type(TM::get("events.safe.trader.pass"));
        return;
    }
    type(TM::get("events.safe.trader.approach"));
    const std::string quitKey = TM::get("events.safe.trader.quit_key");
    const std::string quitPrompt = TM::get("events.safe.trader.quit_prompt", {
        .replacements = {{"{key}", std::to_string(std::tolower(quitKey[0]))}}
    });
    Trader trader;
    player.resources.displayResources();
    trader.displayWares();
    while (true) {
        type (
            TM::get("events.safe.trader.trade.choose_option") +
            TM::getAllAsStr("events.safe.trader.trade.options")
        );

        if (Choice::getChoice(TM::getAllAsLst("events.safe.trade.trade.options")) == 2) {
            type(TM::get("events.safe.trader.trade.leave"));
            return;
        }
        player.resources.displayResources();
        trader.displayWares();
        std::string buyChoice = "";
        do {
            buyChoice = capitalize(input(TM::get("events.safe.trader.trade.buy_res", {
                .replacements = {{"{quit_prompt}", TM::get("events.safe.trader.quit_prompt", {
                    .replacements = {{"{key}", TM::get("events.safe.trader.quit_key")}}
                })}}
            }).c_str()));
            if (buyChoice == quitKey)
                break;
            if (trader.resources[buyChoice] < 1)
                type(TM::get("events.safe.trader.trade.warning.trader_no_res"));
        } while (trader.resources[buyChoice] < 1);
        if (buyChoice == quitKey)
            continue;

        bool noResources = true;
        int amountBuy = 0;
        do {
            std::string strAmount = input(TM::get("events.safe.trader.trade.buy_amt", {.end = ' '}).c_str());
            amountBuy = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
            noResources = (amountBuy == -1 || trader.resources[buyChoice] < amountBuy);
            if (noResources)
                type(TM::get("events.safe.trader.trade.warning.trader_lim_res"));
        } while (noResources);
                
        std::string sellChoice = "";
        do {
            sellChoice = capitalize(input(TM::get("events.safe.trader.trade.sell_res", {
                .replacements = {{"{quit_prompt}", quitPrompt}}
            }).c_str()));
            if (sellChoice == quitKey)
                break;
            if (player.resources[sellChoice] < 1)
                type(TM::get("events.safe.trader.trade.warning.player_no_res"));
            if (player.special.name == sellChoice) {
                type(TM::get("events.safe.trader.trade.warning.player_special"));
                sellChoice = "";
            }
        } while (player.resources[sellChoice] < 1);
        if (sellChoice == quitKey)
            continue;

        int amountSell = 0;
        do {
            std::string strAmount = input(TM::get("events.safe.trader.trade.sell_amt", {.end = ' '}).c_str());
            amountSell = (isInt(strAmount) && isPos(strAmount)) ? stoi(strAmount) : -1;
            noResources = (amountSell == -1 || player.resources[sellChoice] < amountSell);
            if (noResources)
                type(TM::get("events.safe.trader.trade.warning.player_lim_res"));
        } while (noResources);

        if (trader.resources.prices[buyChoice] * amountBuy < trader.resources.prices[sellChoice] * amountSell) { // If the player is not getting a fair trade
            type(TM::get("events.safe.trader.trade.warning.unbalanced"));
            int confirmChoice;
            do confirmChoice = Choice(input(prompt.data())).isChoice({"yes", "no"});
            while (confirmChoice == 0);

            if (confirmChoice == 2)
                continue;
        }

        type(TM::get("events.safe.trader.trade.complete", {
            .replacements = {
                {"{buy_amt}", std::to_string(amountBuy)},
                {"{buy_res}", buyChoice},
                {"{sell_amt}", std::to_string(amountSell)},
                {"{sell_res}", sellChoice}
            }
        }));
        trader.resources[sellChoice] += ui16(amountSell);
        trader.resources[buyChoice] -= ui16(amountBuy);
        player.resources[sellChoice] -= ui16(amountSell);
        player.resources[buyChoice] += ui16(amountBuy);
    }
}

void EventsS::nothing() {
    type(TM::get("events.safe.nothing"));
}

void EventsS::stream() {
    type(TM::get("events.safe.stream"));
    player.heal(3);
    if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
        player.mana++;
}

void EventsS::garden() {
    type(TM::get("events.safe.garden"));
    player.resources.addResource("Medicinal Herbs", randint(1, 3));
    player.heal(3);
    if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
        player.mana++;
}

void EventsS::meadow() {
    type(TM::get("events.safe.meadow"));
    player.heal(3);
    if (player.Class == Player::CLASS::WIZARD && player.mana != player.maxMana)
        player.mana++;
}

void EventsS::statue() {
    //              Þar Ændric, Wensolet Růnanet Tenanek.
    //              In nom Λan, nir χatnůlven. In ten Λan, čenkorek noχardren a'eþ.
    // Translation: King Aendric, the Last Light of the Land of Shadows.
    //              In His name, death was undone. In His shadow, all spirits linger.
    // Literally:   King Aendric, the Last Light of the Land of Shadows.
    //              In His name, death was made void. In His shadow, all spirits are not bound by time.
    type(TM::get("events.safe.statue"));
}