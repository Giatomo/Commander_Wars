#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    class ClipRectActor;
    using spClipRectActor = intrusive_ptr<ClipRectActor>;
    /**
    ClipRectActor clips all out of bound children. Rotation is not supported
    */
    class ClipRectActor : public Actor
    {
    public:
        explicit ClipRectActor() = default;
        virtual ~ClipRectActor() = default;

        bool getClipping() const
        {
            return m_clipping;
        }
        /**Enables clipping for children. By default is on*/
        void setClipping(bool enable)
        {
            m_clipping = enable;
        }
        virtual void render(const RenderState& rs) override;
        virtual void handleEvent(Event* event) override;
    protected:
        bool m_clipping{true};
    };
}
