var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_AIR_TO_AIR_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(9);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("fighter", false);
        unit.loadSprite("fighter+mask", true);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getName = function()
    {
        return qsTr("Fighter");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        unit.setFuel(unit.getFuel() - 5);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+air", 0, -map.getImageSize(), 0, 1.5);
        return animation;
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var FIGHTER = new Constructor();
