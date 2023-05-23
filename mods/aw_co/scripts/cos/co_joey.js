CO_JOEY.superPowerOffMalus = -10;
CO_JOEY.superPowerOffBonus = 30;
CO_JOEY.superPowerBaseOffBonus = 0;
CO_JOEY.superPowerDefBonus = 10;
CO_JOEY.superPowerCostReduction = 0.2;

CO_JOEY.powerOffMalus = -10;
CO_JOEY.powerOffBonus = 30;
CO_JOEY.powerBaseOffBonus = 10;
CO_JOEY.powerDefBonus = 30;
CO_JOEY.powerBaseDefBonus = 10;

CO_JOEY.d2dOffMalus = -10;
CO_JOEY.d2dOffBonus = 20;
CO_JOEY.d2dBaseOffBonus = 0;

CO_JOEY.d2dCoZoneOffMalus = 0;
CO_JOEY.d2dCoZoneOffBonus = 0;
CO_JOEY.d2dCoZoneBaseOffBonus = 0;

CO_JOEY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_JOEY.activateSuperpower = function(co, powerMode, map)
{
    CO_JOEY.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
CO_JOEY.getSuperPowerDescription = function()
{
    return CO_JOEY.getPowerDescription();
};
CO_JOEY.getSuperPowerName = function()
{
    return CO_JOEY.getPowerName();
};
