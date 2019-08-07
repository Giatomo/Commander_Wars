var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(SNOW_MOUNTAIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Mountain");
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SNOW");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("snow_mountain");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_mountain";
    };
    this.getBonusVision = function(unit)
    {
        var infantry = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        if (infantry.indexOf(unit.getUnitID()) >= 0)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_mountain";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_mountain";
    };
    this.getDescription = function()
    {
        return qsTr("Clear view. In Fog of War, Infantry unit's gain <div c='00ff00'>vision +3.</div> Extremly high movement costs for infantry units.");
    };
};
Constructor.prototype = TERRAIN;
var SNOW_MOUNTAIN = new Constructor();
