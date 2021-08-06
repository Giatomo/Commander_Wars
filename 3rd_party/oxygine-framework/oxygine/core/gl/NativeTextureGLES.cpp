#include "3rd_party/oxygine-framework/oxygine/core/gl/NativeTextureGLES.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"

#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#   define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#endif

namespace oxygine
{
    struct glPixel
    {
        qint32 format;
        qint32 type;
        bool compressed;
    };

    glPixel SurfaceFormat2GL(ImageData::TextureFormat format)
    {
        glPixel pixel;
        pixel.compressed = false;
        switch (format)
        {
            case ImageData::TF_R8G8B8A8:
                pixel.format = GL_RGBA;
                pixel.type = GL_UNSIGNED_BYTE;
                break;
            default:
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "unknown format: " + QString::number(format));
        }
        return pixel;
    }

    GLuint createTexture()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLuint ids[1] = {0};
        window->glGenTextures(1, ids);
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, ids[0]);

        GLint  f = GL_LINEAR;
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return ids[0];
    }

    NativeTextureGLES::NativeTextureGLES()
        : m_id(0),
          m_format(ImageData::TF_UNDEFINED),
          m_width(0),
          m_height(0),
          m_lockFlags(0)
    {

    }

    void NativeTextureGLES::init(qint32 w, qint32 h, ImageData::TextureFormat tf)
    {
        release();

        GameWindow* window = oxygine::GameWindow::getWindow();

        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(tf);
        window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, w, h, 0, p.format, p.type, 0);        

        m_id = id;
        m_width = w;
        m_height = h;
        m_format = tf;
    }

    void NativeTextureGLES::init(GLuint id, qint32 w, qint32 h, ImageData::TextureFormat tf)
    {
        release();
        m_id = id;
        m_width = w;
        m_height = h;
        m_format = tf;
    }

    void NativeTextureGLES::init(const ImageData& src)
    {
        GLuint id = createTexture();

        glPixel p = SurfaceFormat2GL(src.m_format);
        GameWindow* window = oxygine::GameWindow::getWindow();

        if (p.compressed)
        {
            window->glCompressedTexImage2D(GL_TEXTURE_2D, 0, p.format, src.m_w, src.m_h, 0, src.m_h * src.m_pitch, src.m_data);
        }
        else
        {
            window->glTexImage2D(GL_TEXTURE_2D, 0, p.format, src.m_w, src.m_h, 0, p.format, p.type, src.m_data);
        }
        init(id, src.m_w, src.m_h, src.m_format);
    }

    void NativeTextureGLES::setLinearFilter(quint32 filter)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        window->glGenerateMipmap(GL_TEXTURE_2D);
    }

    void NativeTextureGLES::setClamp2Edge(bool clamp2edge)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

        GLint f = clamp2edge ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
        window->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
    }

    void NativeTextureGLES::release()
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        if (m_id)
        {
            GLuint ids[] = {m_id};
            window->glDeleteTextures(1, ids);
            m_id = 0;
        }       
        unreg();
    }


    void NativeTextureGLES::swap(NativeTexture* t)
    {
        NativeTextureGLES* nt = safeCast<NativeTextureGLES*>(t);
        std::swap(m_id, nt->m_id);
        std::swap(m_format, nt->m_format);
        std::swap(m_width, nt->m_width);
        std::swap(m_height, nt->m_height);
        std::swap(m_format, nt->m_format);
    }

    NativeTextureGLES::~NativeTextureGLES()
    {
        NativeTextureGLES::release();
    }

    qint32 NativeTextureGLES::getWidth() const
    {
        return m_width;
    }

    qint32 NativeTextureGLES::getHeight() const
    {
        return m_height;
    }

    ImageData::TextureFormat NativeTextureGLES::getFormat() const
    {
        return m_format;
    }

    ImageData NativeTextureGLES::lock(lock_flags flags, const Rect* src)
    {
        if (m_lockFlags != 0)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "NativeTextureGLES::lock already locked");
            return ImageData();
        }
        m_lockFlags = flags;
        Rect r(0, 0, m_width, m_height);

        if (src)
        {
            r = *src;
        }
        if (r.getX() + r.getWidth() > m_width ||
            r.getY() + r.getHeight() > m_height)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "NativeTextureGLES::lock invalid copy rect");
            return ImageData();
        }

        m_lockRect = r;
        if (m_lockFlags == 0)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "NativeTextureGLES::lock called with invalid lock flags");
            return ImageData();
        }

        if (m_lockRect.isEmpty())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "NativeTextureGLES::lock _lockRect.IsEmpty()");
            return ImageData();
        }

        ImageData im =  ImageData(m_width, m_height, (int)(m_data.size() / m_height), m_format, &m_data.front());
        return im.getRect(m_lockRect);
    }

    void NativeTextureGLES::unlock()
    {
        if (!m_lockFlags)
        {
            return;
        }
        if (m_lockFlags & lock_write)
        {
            GameWindow* window = oxygine::GameWindow::getWindow();
            window->glActiveTexture(GL_TEXTURE7);
            window->glBindTexture(GL_TEXTURE_2D, m_id);
            window->glGetError();
            ImageData src = ImageData(m_width, m_height, static_cast<qint32>(m_data.size() / m_height), m_format, &m_data.front());
            ImageData locked = src.getRect(m_lockRect);
            window->glGetError();
            Image mt;
            mt.init(m_lockRect.getWidth(), m_lockRect.getHeight(), m_format);
            ImageData q = mt.lock();
            operations::copy(locked, q);
            mt.unlock();

            glPixel glp = SurfaceFormat2GL(m_format);

            window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                    m_lockRect.getX(), m_lockRect.getY(), m_lockRect.getWidth(), m_lockRect.getHeight(),
                                    glp.format, glp.type, locked.m_data);
            window->glGetError();
            m_lockFlags = 0;
        }
    }

    void NativeTextureGLES::updateRegion(qint32 x, qint32 y, const ImageData& data_)
    {
        if (data_.m_w == 0 || data_.m_h == 0)
            return;

        ImageData data = data_;
        assert(m_width >= data.m_w - x);
        assert(m_height >= data.m_h - y);
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glActiveTexture(GL_TEXTURE7);
        window->glBindTexture(GL_TEXTURE_2D, (GLuint) m_id);

        glPixel glp = SurfaceFormat2GL(m_format);
        Image mt;
        if (m_format != data.m_format)
        {
            mt.init(data.m_w, data.m_h, m_format);
            mt.fillZero();
            mt.updateRegion(0, 0, data);
            data = mt.lock();
        }

        window->glTexSubImage2D(GL_TEXTURE_2D, 0,
                                x, y, data.m_w, data.m_h,
                                glp.format, glp.type, data.m_data);
    }

    void NativeTextureGLES::apply(const Rect*)
    {

    }

    GLuint NativeTextureGLES::getHandle() const
    {
        return reinterpret_cast<GLuint>(m_id);
    }
}
