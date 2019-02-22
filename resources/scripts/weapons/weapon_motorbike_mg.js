var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("MG");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
        case "INFANTRY":
            return WEAPON.lowDamage;
        case "MECH":
            return WEAPON.HighDamage;
        case "MOTORBIKE":
            return WEAPON.mediumDamage;
        case "SNIPER":
            return WEAPON.HighDamage;

        case "APC":
            return 20;
        case "FLARE":
            return 20;
        case "RECON":
            return 20;

        case "FLAK":
            return 5;
        case "HOVERFLAK":
            return 5;
        case "LIGHT_TANK":
            return 5;
        case "HOVERFLAK":
            return 5;

        case "HEAVY_HOVERCRAFT":
            return 3;
        case "HEAVY_TANK":
            return 3;
        case "NEOTANK":
            return 3;

        case "MEGATANK":
            return 1;

        case "T_HELI":
            return 30;
        case "K_HELI":
            return 10;

        case "ARTILLERY":
            return 15;
        case "ANITANKCANNON":
            return 35;
        case "MISSILE":
            return 35;
        case "ROCKETTRHOWER":
            return 35;
        default:
            return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_MOTORBIKE_MG = new Constructor();
