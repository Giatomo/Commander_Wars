#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

#include <QOpenGLShader>

#include <QImage>

namespace oxygine
{
class Texture;
using spTexture = intrusive_ptr<Texture>;
class Texture final : public QObject, public ref_counter
{
    Q_OBJECT
public:
    virtual ~Texture();
    void init(const QImage & image);

    qint32 getWidth() const
    {
        return m_image.width();
    }
    qint32 getHeight() const
    {
        return m_image.height();
    }
    quint32 getHandle() const
    {
        return m_id;
    }
    /**Disabled/Enables bilinear texture filter*/
    void setLinearFilter(quint32 filter);
    /**set wrap mode as clamp2edge or repeat (tiling)*/
    void setClamp2Edge(bool clamp2edge);

    timeMS getCreationTime()
    {
        return m_CreationTime;
    }
    void setCreationTime(timeMS time)
    {
        m_CreationTime = time;
    }
    void release();
    static quint32 getHighestTextureCount();
    const QImage & getImage() const
    {
        return m_image;
    }
protected:
    friend class VideoDriver;
    friend class intrusive_ptr<Texture>;
    explicit Texture();
    GLuint createTexture();
private:
    GLuint m_id{0};

    QImage m_image;
    timeMS m_CreationTime;
    static GLuint m_highestTextureCount;
};
}
