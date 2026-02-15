#include "Config.h"
#include "Define.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "InstanceSaveMgr.h"
#include "Chat.h"

enum PremiumTeleports
{
    TELEPORT_MENU = GOSSIP_ACTION_INFO_DEF + 30,

    // Alliance
    TELEPORT_STORMWIND = GOSSIP_ACTION_INFO_DEF + 31,
    TELEPORT_IRONFORGE = GOSSIP_ACTION_INFO_DEF + 32,
    TELEPORT_DARNASSUS = GOSSIP_ACTION_INFO_DEF + 33,

    // Horde
    TELEPORT_ORGRIMMAR = GOSSIP_ACTION_INFO_DEF + 34,
    TELEPORT_THUNDERBLUFF = GOSSIP_ACTION_INFO_DEF + 35,
    TELEPORT_UNDERCITY = GOSSIP_ACTION_INFO_DEF + 36,
    TELEPORT_ICC = GOSSIP_ACTION_INFO_DEF + 37,
    TELEPORT_ULDUAR = GOSSIP_ACTION_INFO_DEF + 38,
    TELEPORT_DALARAN = GOSSIP_ACTION_INFO_DEF + 39
};
enum Vendors
{
    NPC_VENDOR_A = 54,
    NPC_VENDOR_H = 3163,
    NPC_AUCTION_H = 9856,
    NPC_AUCTION_A = 8670
};

enum Trainers
{
    // Alliance
    DRUID_A = 5504,
    HUNTER_A = 5515,
    MAGE_A = 5497,
    PALADIN_A = 928,
    PRIEST_A = 376,
    ROGUE_A = 918,
    SHAMAN_A = 20407,
    WARLOCK_A = 461,
    WARRIOR_A = 5479,

    // Horde
    DRUID_H = 3033,
    HUNTER_H = 3406,
    MAGE_H = 5883,
    PALADIN_H = 23128,
    PRIEST_H = 3045,
    ROGUE_H = 3401,
    SHAMAN_H = 3344,
    WARLOCK_H = 3324,
    WARRIOR_H = 3354,

    DEATHKNIGHT_AH = 28472
};

enum Mounts
{
    HUMAN_MOUNT     = 470,
    ORC_MOUNT       = 6653,
    GNOME_MOUNT     = 17454,
    NIGHTELF_MOUNT  = 8394,
    DWARF_MOUNT     = 6899,
    UNEAD_MOUNT     = 17463,
    TAUREN_MOUNT    = 64657,
    TROLL_MOUNT     = 8395,
    BLOODELF_MOUNT  = 35022,
    DRAENEI_MOUNT   = 34406
};

enum PremiumGossip
{
    PREMIUM_MENU = 62001,
    PREMIUM_MENU_TEXT = 90003,
    GOSSIP_MORPH = 0,
    GOSSIP_DEMORPH,
    GOSSIP_MOUNT,
    GOSSIP_TRAIN_ME,
    GOSSIP_PLAYER,
    GOSSIP_VENDOR,
    GOSSIP_MAIL,
    GOSSIP_BANK,
    GOSSIP_AUCTION_HOUSE,
    GOSSIP_FACTION
};
enum PremiumActions
{
    PREMIUM_RESET_INSTANCES = GOSSIP_ACTION_INFO_DEF + 50,
    PREMIUM_LEAVE_COMBAT = GOSSIP_ACTION_INFO_DEF + 51,
    PREMIUM_RESET_TALENTS = GOSSIP_ACTION_INFO_DEF + 52,
    PREMIUM_OPEN_BANK = GOSSIP_ACTION_INFO_DEF + 53,
    PREMIUM_OPEN_MAIL = GOSSIP_ACTION_INFO_DEF + 54
};

class premium_account : public ItemScript
{
public:
    premium_account() : ItemScript("premium_account") {}

    bool OnUse(Player *player, Item *item, SpellCastTargets const & /*targets*/) override
    {
        if (!sConfigMgr->GetOption<bool>("PremiumAccount", true))
            return false;

        QueryResult result = CharacterDatabase.Query("SELECT `AccountId` FROM `premium` WHERE `active`=1 AND `AccountId`={}", player->GetSession()->GetAccountId());

        if (!result)
            return false;

        if (player->IsInCombat())
            return false;

        float rangeCheck = 10.0f;

        if (player->FindNearestCreature(NPC_AUCTION_A, rangeCheck) ||
            player->FindNearestCreature(NPC_AUCTION_H, rangeCheck) ||
            player->FindNearestCreature(NPC_VENDOR_A, rangeCheck) ||
            player->FindNearestCreature(NPC_VENDOR_H, rangeCheck) ||
            player->FindNearestCreature(ROGUE_A, rangeCheck) ||
            player->FindNearestCreature(WARRIOR_A, rangeCheck) ||
            player->FindNearestCreature(HUNTER_A, rangeCheck) ||
            player->FindNearestCreature(PRIEST_A, rangeCheck) ||
            player->FindNearestCreature(PALADIN_A, rangeCheck) ||
            player->FindNearestCreature(DRUID_A, rangeCheck) ||
            player->FindNearestCreature(SHAMAN_A, rangeCheck) ||
            player->FindNearestCreature(MAGE_A, rangeCheck) ||
            player->FindNearestCreature(WARLOCK_A, rangeCheck) ||
            player->FindNearestCreature(HUNTER_H, rangeCheck) ||
            player->FindNearestCreature(WARRIOR_H, rangeCheck) ||
            player->FindNearestCreature(SHAMAN_H, rangeCheck) ||
            player->FindNearestCreature(PALADIN_H, rangeCheck) ||
            player->FindNearestCreature(ROGUE_H, rangeCheck) ||
            player->FindNearestCreature(WARLOCK_H, rangeCheck) ||
            player->FindNearestCreature(MAGE_H, rangeCheck) ||
            player->FindNearestCreature(PRIEST_H, rangeCheck) ||
            player->FindNearestCreature(DRUID_H, rangeCheck) ||
            player->FindNearestCreature(DEATHKNIGHT_AH, rangeCheck))
            return false;

        ClearGossipMenuFor(player);

        if (sConfigMgr->GetOption<bool>("Morph", true))
        {
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MORPH, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_DEMORPH, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        }

        if (sConfigMgr->GetOption<bool>("Mount", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MOUNT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

        if (sConfigMgr->GetOption<bool>("Trainers", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_TRAIN_ME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);

        if (sConfigMgr->GetOption<bool>("PlayerInteraction", true))
            AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_PLAYER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
        AddGossipItemFor(
            player,
            GOSSIP_ICON_CHAT,
            "Reset Instances",
            GOSSIP_SENDER_MAIN,
            PREMIUM_RESET_INSTANCES);
        AddGossipItemFor(
            player,
            GOSSIP_ICON_CHAT,
            "Teleport",
            GOSSIP_SENDER_MAIN,
            TELEPORT_MENU);
        AddGossipItemFor(
            player,
            GOSSIP_ICON_CHAT,
            "Leave Combat",
            GOSSIP_SENDER_MAIN,
            PREMIUM_LEAVE_COMBAT);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Reset Talents", GOSSIP_SENDER_MAIN, PREMIUM_RESET_TALENTS);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Bank", GOSSIP_SENDER_MAIN, PREMIUM_OPEN_BANK);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Mail", GOSSIP_SENDER_MAIN, PREMIUM_OPEN_MAIL);
        SendGossipMenuFor(player, PREMIUM_MENU_TEXT, item->GetGUID());
        return true; // Cast the spell on use normally
    }

    void OnGossipSelect(Player *player, Item *item, uint32 /*sender*/, uint32 action) override
    {
        switch (action)
        {
        case PREMIUM_OPEN_BANK:
        {
            CloseGossipMenuFor(player);
            player->GetSession()->SendShowBank(player->GetGUID());
            break;
        }
        case PREMIUM_OPEN_MAIL:
        {
            CloseGossipMenuFor(player);
            player->GetSession()->SendShowMailBox(player->GetGUID());
            break;
        }

        case PREMIUM_RESET_TALENTS:
        {
            CloseGossipMenuFor(player);

            if (player->IsInCombat())
            {
                ChatHandler(player->GetSession()).SendNotification("No puedes resetear talentos en combate.");
                break;
            }

            player->resetTalents(true);
            player->SendTalentsInfoData(false);

            ChatHandler(player->GetSession()).SendNotification("Talentos reiniciados correctamente.");

            player->CastSpell(player, 31726);
            break;
        }

        case PREMIUM_LEAVE_COMBAT:
        {
            CloseGossipMenuFor(player);

            if (!player->IsInCombat())
            {
                ChatHandler(player->GetSession()).SendNotification("No estas en combate.");
                break;
            }

            player->CombatStop(true);
            player->ClearInCombat();
            player->RemoveAllAttackers();

            ChatHandler(player->GetSession()).SendNotification("Combate eliminado correctamente.");

            player->CastSpell(player, 31726);
            break;
        }
        case PREMIUM_RESET_INSTANCES:
        {
            CloseGossipMenuFor(player);

            if (player->IsInCombat())
            {
                ChatHandler(player->GetSession()).SendNotification("No puedes resetear instancias en combate.");
                return;
            }

            for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
            {
                BoundInstancesMap const &boundInstances =
                    sInstanceSaveMgr->PlayerGetBoundInstances(
                        player->GetGUID(),
                        Difficulty(i));

                for (auto itr = boundInstances.begin(); itr != boundInstances.end();)
                {
                    if (itr->first != player->GetMapId())
                    {
                        sInstanceSaveMgr->PlayerUnbindInstance(
                            player->GetGUID(),
                            itr->first,
                            Difficulty(i),
                            true,
                            player);
                        itr = boundInstances.begin();
                    }
                    else
                        ++itr;
                }
            }

            ChatHandler(player->GetSession()).SendNotification("Instancias reiniciadas correctamente.");

            player->CastSpell(player, 59908);
            break;
        }

        case TELEPORT_MENU:
        {
            ClearGossipMenuFor(player);

            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Stormwind", GOSSIP_SENDER_MAIN, TELEPORT_STORMWIND);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Ironforge", GOSSIP_SENDER_MAIN, TELEPORT_IRONFORGE);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Darnassus", GOSSIP_SENDER_MAIN, TELEPORT_DARNASSUS);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "ICC", GOSSIP_SENDER_MAIN, TELEPORT_ICC);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Ulduar", GOSSIP_SENDER_MAIN, TELEPORT_ULDUAR);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Dalaran", GOSSIP_SENDER_MAIN, TELEPORT_DALARAN);
            }
            else
            {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Orgrimmar", GOSSIP_SENDER_MAIN, TELEPORT_ORGRIMMAR);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Thunder Bluff", GOSSIP_SENDER_MAIN, TELEPORT_THUNDERBLUFF);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Undercity", GOSSIP_SENDER_MAIN, TELEPORT_UNDERCITY);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "ICC", GOSSIP_SENDER_MAIN, TELEPORT_ICC);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Ulduar", GOSSIP_SENDER_MAIN, TELEPORT_ULDUAR);
                AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Dalaran", GOSSIP_SENDER_MAIN, TELEPORT_DALARAN);
            }

            SendGossipMenuFor(player, PREMIUM_MENU_TEXT, item->GetGUID());
            break;
        }
        case TELEPORT_DALARAN:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(
                571,        // Map
                5805.3013f, // X
                623.0144f,  // Y
                647.8444f,  // Z
                1.5371346f  // Orientation
            );
            player->SetHomebind(
                WorldLocation(571, 5805.3013f, 623.0144f, 647.8444f, 1.5371346f),
                4395 // ZoneId Dalaran
            );
            break;
        }

        case TELEPORT_ULDUAR:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 9227.008f, -1109.4019f, 1216.116f, 6.0428348f);
            player->SetHomebind(
                WorldLocation(571, 9227.008f, -1109.4019f, 1216.116f, 6.0428348f),
                4445);
            break;
        }
        case TELEPORT_ICC:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(571, 5803.36f, 2078.1f, 636.06494f, 0.459897f);
            player->SetHomebind(
                WorldLocation(571, 5803.36f, 2078.1f, 636.06494f, 0.459897f),
                4522);
            break;
        }
        case TELEPORT_STORMWIND:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -8833.38f, 628.62f, 94.00f, 1.06f);
            player->SetHomebind(WorldLocation(0, -8866.55f, 671.39f, 97.90f, 5.27f), 1637);
            break;
        }

        case TELEPORT_IRONFORGE:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, -4981.25f, -881.54f, 501.66f, 5.40f);
            player->SetHomebind(WorldLocation(0, -4981.25f, -881.54f, 501.66f, 5.40f), 1537);
            break;
        }

        case TELEPORT_DARNASSUS:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 9951.52f, 2280.32f, 1341.39f, 1.59f);
            player->SetHomebind(WorldLocation(1, 9951.52f, 2280.32f, 1341.39f, 1.59f), 1657);
            break;
        }

        case TELEPORT_ORGRIMMAR:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, 1569.59f, -4397.63f, 7.70f, 0.54f);
            player->SetHomebind(WorldLocation(1, 1637.62f, -4440.22f, 15.78f, 2.42f), 1653);
            break;
        }

        case TELEPORT_THUNDERBLUFF:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(1, -1277.37f, 124.80f, 131.29f, 5.22f);
            player->SetHomebind(WorldLocation(1, -1277.37f, 124.80f, 131.29f, 5.22f), 1638);
            break;
        }

        case TELEPORT_UNDERCITY:
        {
            CloseGossipMenuFor(player);
            player->TeleportTo(0, 1632.84f, 240.35f, -43.10f, 6.26f);
            player->SetHomebind(WorldLocation(0, 1632.84f, 240.35f, -43.10f, 6.26f), 1497);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 1: /*Morph*/
        {
            CloseGossipMenuFor(player);
            ApplyRandomMorph(player);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 2: /*Demorph*/
        {
            player->DeMorph();
            CloseGossipMenuFor(player);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 3: /*Show Bank*/
        {
            player->GetSession()->SendShowBank(player->GetGUID());
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 4: /*Mail Box*/
        {
            player->GetSession()->SendShowMailBox(player->GetGUID());
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 5: /*Vendor*/
        {
            uint32 vendorId = 0;
            std::string salute;

            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                vendorId = NPC_VENDOR_A;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                case LOCALE_enUS:
                case LOCALE_koKR:
                case LOCALE_frFR:
                case LOCALE_deDE:
                case LOCALE_zhCN:
                case LOCALE_zhTW:
                case LOCALE_ruRU:
                {
                    salute = "Greetings";
                    break;
                }
                case LOCALE_esES:
                case LOCALE_esMX:
                {
                    salute = "Saludos.";
                }
                default:
                    break;
                }
            }
            else
            {
                vendorId = NPC_VENDOR_H;
                salute = "Zug zug";
            }

            SummonTempNPC(player, vendorId, salute.c_str());
            CloseGossipMenuFor(player);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 6: /*Mount*/
        {
            CloseGossipMenuFor(player);
            switch (player->getRace())
            {
            case RACE_HUMAN:
                player->CastSpell(player, HUMAN_MOUNT);
                break;
            case RACE_ORC:
                player->CastSpell(player, ORC_MOUNT);
                break;
            case RACE_GNOME:
                player->CastSpell(player, GNOME_MOUNT);
                break;
            case RACE_NIGHTELF:
                player->CastSpell(player, NIGHTELF_MOUNT);
                break;
            case RACE_DWARF:
                player->CastSpell(player, DWARF_MOUNT);
                break;
            case RACE_DRAENEI:
                player->CastSpell(player, DRAENEI_MOUNT);
                break;
            case RACE_UNDEAD_PLAYER:
                player->CastSpell(player, UNEAD_MOUNT);
                break;
            case RACE_TAUREN:
                player->CastSpell(player, TAUREN_MOUNT);
                break;
            case RACE_TROLL:
                player->CastSpell(player, TROLL_MOUNT);
                break;
            case RACE_BLOODELF:
                player->CastSpell(player, BLOODELF_MOUNT);
                break;
            }
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 7: /*Auction House*/
        {
            uint32 auctionId = 0;
            std::string salute = "";
            if (player->GetTeamId() == TEAM_HORDE)
            {
                auctionId = NPC_AUCTION_H;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                case LOCALE_enUS:
                case LOCALE_koKR:
                case LOCALE_frFR:
                case LOCALE_deDE:
                case LOCALE_zhCN:
                case LOCALE_zhTW:
                case LOCALE_ruRU:
                {
                    salute = "I will go shortly, I need to get back to Orgrimmar";
                    break;
                }
                case LOCALE_esES:
                case LOCALE_esMX:
                {
                    salute = "Me iré en breve, necesito volver a Orgrimmar.";
                }
                default:
                    break;
                }
            }
            else
            {
                auctionId = NPC_AUCTION_A;
                switch (player->GetSession()->GetSessionDbLocaleIndex())
                {
                case LOCALE_enUS:
                case LOCALE_koKR:
                case LOCALE_frFR:
                case LOCALE_deDE:
                case LOCALE_zhCN:
                case LOCALE_zhTW:
                case LOCALE_ruRU:
                {
                    salute = "I will go shortly, I need to get back to Stormwind City";
                    break;
                }
                case LOCALE_esES:
                case LOCALE_esMX:
                {
                    salute = "Me iré en breve, necesito volver a la Ciudad de Ventormenta.";
                }
                default:
                    break;
                }
            }

            SummonTempNPC(player, auctionId, salute.c_str());
            CloseGossipMenuFor(player);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 8: /* Class Trainers*/
        {
            uint32 trainerId = 0;
            switch (player->getClass())
            {
            case CLASS_ROGUE:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? ROGUE_A : ROGUE_H;
                break;
            case CLASS_WARRIOR:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARRIOR_A : WARRIOR_H;
                break;
            case CLASS_PRIEST:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PRIEST_A : PRIEST_H;
                break;
            case CLASS_MAGE:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? MAGE_A : MAGE_H;
                break;
            case CLASS_PALADIN:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PALADIN_A : PALADIN_H;
                break;
            case CLASS_HUNTER:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? HUNTER_A : HUNTER_H;
                break;
            case CLASS_DRUID:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? DRUID_A : DRUID_H;
                break;
            case CLASS_SHAMAN:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? SHAMAN_A : SHAMAN_H;
                break;
            case CLASS_WARLOCK:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARLOCK_A : WARLOCK_H;
                break;
            case CLASS_DEATH_KNIGHT:
                trainerId = DEATHKNIGHT_AH;
                break;
            }

            SummonTempNPC(player, trainerId);
            CloseGossipMenuFor(player);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 9: /*Player Interactions*/
        {
            ClearGossipMenuFor(player);

            if (sConfigMgr->GetOption<bool>("Vendor", true))
                AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_VENDOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);

            if (sConfigMgr->GetOption<bool>("MailBox", true))
                AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_MAIL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);

            if (sConfigMgr->GetOption<bool>("Bank", true))
                AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_BANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

            if (sConfigMgr->GetOption<bool>("Auction", true))
                AddGossipItemFor(player, PREMIUM_MENU, GOSSIP_AUCTION_HOUSE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

            SendGossipMenuFor(player, PREMIUM_MENU_TEXT, item->GetGUID());
            break;
        }
        }
    }

    void ApplyRandomMorph(Player *player)
    {
        uint32 random = (urand(1, 26)); // Change this line when adding more morphs
        {
            switch (random)
            {
            case 1:
                player->SetDisplayId(10134);
                break; // Troll Female                 'Orb of Deception'
            case 2:
                player->SetDisplayId(10135);
                break; // Troll Male                   'Orb of Deception'
            case 3:
                player->SetDisplayId(10136);
                break; // Tauren Male                  'Orb of Deception'
            case 4:
                player->SetDisplayId(10137);
                break; // Human Male                   'Orb of Deception'
            case 5:
                player->SetDisplayId(10138);
                break; // Human Female                 'Orb of Deception'
            case 6:
                player->SetDisplayId(10139);
                break; // Orc Male                     'Orb of Deception'
            case 7:
                player->SetDisplayId(10140);
                break; // Orc Female                   'Orb of Deception'
            case 8:
                player->SetDisplayId(10141);
                break; // Dwarf Male                   'Orb of Deception'
            case 9:
                player->SetDisplayId(10142);
                break; // Dwarf Female                 'Orb of Deception'
            case 10:
                player->SetDisplayId(10143);
                break; // NightElf Male                'Orb of Deception'
            case 11:
                player->SetDisplayId(10144);
                break; // NightElf Female              'Orb of Deception'
            case 12:
                player->SetDisplayId(10145);
                break; // Undead Female                'Orb of Deception'
            case 13:
                player->SetDisplayId(10146);
                break; // Undead Male                  'Orb of Deception'
            case 14:
                player->SetDisplayId(10147);
                break; // Tauren Female                'Orb of Deception'
            case 15:
                player->SetDisplayId(10148);
                break; // Gnome Male                   'Orb of Deception'
            case 16:
                player->SetDisplayId(10149);
                break; // Gnome Female                 'Orb of Deception'
            case 17:
                player->SetDisplayId(4527);
                break; // Thrall                       'Orgrimmar Boss'
            case 18:
                player->SetDisplayId(11657);
                break; // Lady Sylvanas                'Undercity Boss'
            case 19:
                player->SetDisplayId(4307);
                break; // Cairne Bloodhoof             'Thunderbluff Boss'
            case 20:
                player->SetDisplayId(17122);
                break; // Lor'themar Theron            'Silvermoon City Boss'
            case 21:
                player->SetDisplayId(3597);
                break; // King Magni Bronzebeard       'Ironforge Boss'
            case 22:
                player->SetDisplayId(5566);
                break; // Highlord Bolvar Fordragon    'Stormwind Boss'
            case 23:
                player->SetDisplayId(7006);
                break; // High Tinker Mekkatorque      'Gnomer Boss'
            case 24:
                player->SetDisplayId(7274);
                break; // Tyrande Whisperwind          'Darnassus Boss'
            case 25:
                player->SetDisplayId(21976);
                break; // Arthus Small                 'Arthus'
            case 26:
                player->SetDisplayId(24641);
                break; // Arthus Ghost                 'Arthus Ghost'

            default:
                break;
            }
        }
    }

    void SummonTempNPC(Player *player, uint32 entry, const char *salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sConfigMgr->GetOption<int32>("Premium.NpcDuration", 60) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 60;

        Creature *npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
        npc->SetFaction(player->GetFaction());

        if (salute && !(salute[0] == '\0'))
            npc->Whisper(salute, LANG_UNIVERSAL, player, false);
    }
};

void AddPremiumAccount()
{
    new premium_account();
}
