var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Air");
    };
    this.movementpointsTable = [["PLAINS", 1],
                                ["PLAINS_DESTROYED", 1],
                                ["PLAINS_PLASMA", 1],
                                ["BEACH", 1],
                                ["BRIDGE", 1],
                                ["BRIDGE1", 1],
                                ["BRIDGE2", 1],
                                ["DESTROYEDWELD", 1],
                                ["RUIN", 1],
                                ["STREET", 1],
                                ["SNOW_STREET", 1],
                                ["STREET1", 1],
                                ["AIRPORT", 1],
                                ["FACTORY", 1],
                                ["HARBOUR", 1],
                                ["HQ", 1],
                                ["MINE", 1],
                                ["PIPESTATION", 1],
                                ["POWERPLANT", 1],
                                ["AMPHIBIOUSFACTORY", 1],
                                ["RADAR", 1],
                                ["TOWER", 1],
                                ["TOWN", 1],
                                ["SILO", 1],
                                ["SILO_ROCKET", 1],
                                ["LABOR", 1],
                                ["DEPOT", 1],
                                ["TEMPORARY_AIRPORT", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["OILRIG", 1],
                                ["ZDESTROYED_GATE_E_W", 1],
                                ["ZDESTROYED_GATE_N_S", 1],
                                ["FOREST", 1],
                                ["FOREST1", 1],
                                ["FOREST2", 1],
                                ["FOREST3", 1],
                                ["WASTELAND", 1],
                                ["SEA", 1],
                                ["LAKE", 1],
                                ["ROUGH_SEA", 1],
                                ["REAF", 1],
                                ["FOG", 1],
                                ["MOUNTAIN", 1],
                                ["RIVER", 1],
                                ["DESERT", 1],
                                ["DESERT_DESTROYEDWELD", 1],
                                ["DESERT_FOREST", 1],
                                ["DESERT_FOREST1", 1],
                                ["DESERT_PATH", 1],
                                ["DESERT_PATH1", 1],
                                ["DESERT_ROCK", 1],
                                ["DESERT_TRY_RIVER", 1],
                                ["DESERT_WASTELAND", 1],
                                ["DESERT_RUIN", 1],
                                ["SNOW", 1],
                                ["SNOW_DESTROYEDWELD", 1],
                                ["SNOW_FOREST", 1],
                                ["SNOW_FOREST1", 1],
                                ["SNOW_FOREST2", 1],
                                ["SNOW_MOUNTAIN", 1],
                                ["SNOW_WASTELAND", 1],
                                ["SNOW_RUIN", 1],
                                ["WASTE",  1],
                                ["WASTE_PATH", 1],
                                ["WASTE_FOREST", 1],
                                ["WASTE_MOUNTAIN", 1],
                                ["WASTE_WASTELAND", 1],
                                ["WASTE_RUIN", 1],
                                ["WASTE_DESTROYEDWELD", 1],
                                ["CREEPER", 1],
                                ["TELEPORTTILE", 0],
                                ["ZGATE_E_W", 1],
                                ["ZGATE_N_S", 1],];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false, map)
    {
        var id = terrain.getID();
        if (id === "ZGATE_E_W" || id === "ZGATE_N_S" || id === "FORTHQ")
        {
            if ((unit !== null) && (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
        return MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_AIR.movementpointsTable);
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_AIR = new Constructor();
