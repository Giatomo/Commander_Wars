var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 5;
    };
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("nest_laser+destroyed", GameEnums.Recoloring_Matrix, 400, Qt.point(0, building.getImageSize()));
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getName = function()
    {
        return qsTr("Nest");
    };
    this.getDescription = function()
    {
        return qsTr("Destroyed nest laser.");
    };
    this.getBuildingWidth = function()
    {
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZNEST_NEST_LASER.getBuildingWidth(), ZNEST_NEST_LASER.getBuildingHeigth(), map);
    };
    this.usesMapLayer = function()
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var ZNEST_NEST_LASER_DESTROYED = new Constructor();
