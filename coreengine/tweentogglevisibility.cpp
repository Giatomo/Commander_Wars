#include "coreengine/tweentogglevisibility.h"

#include "oxygine/actor/Sprite.h"

TweenToggleVisibility::TweenToggleVisibility(float start, float end)
    : startFraction(start),
      endFracrtion(end)
{

}


void TweenToggleVisibility::update(oxygine::Sprite& actor, float p, const oxygine::UpdateState&)
{
    if ((p >= startFraction) && (p <= endFracrtion))
    {
        actor.setVisible(true);
    }
    else
    {
        actor.setVisible(false);
    }
}
