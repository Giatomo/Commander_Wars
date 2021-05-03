#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"

namespace oxygine
{
    namespace text
    {
        Aligner::Aligner(const TextStyle& Style, spSTDMaterial mt, const Font* font, float gscale, const Vector2& size)
            : style(Style),
              bounds(0, 0, 0, 0),
              width((int)size.x),
              height((int)size.y),
              mat(mt),
              _font(font),
              _scale(gscale),
              m_x(0),
              _y(0),
              _lineWidth(0)
        {
            _line.reserve(50);
            _lineSkip = (int)(_font->getBaselineDistance() * style.baselineScale) + style.linesOffset;
            options = Style.options;
        }

        Aligner::~Aligner()
        {

        }

        qint32 Aligner::_alignX(qint32 rx)
        {
            qint32 tx = 0;
            switch (getStyle().hAlign)
            {
                case TextStyle::HALIGN_LEFT:
                case TextStyle::HALIGN_DEFAULT:
                    tx = 0;
                    break;
                case TextStyle::HALIGN_MIDDLE:
                    tx = width / 2 - rx / 2;
                    break;
                case TextStyle::HALIGN_RIGHT:
                    tx = width - rx;
                    break;
            }
            return tx;
        }

        qint32 Aligner::_alignY(qint32 ry)
        {
            qint32 ty = 0;

            switch (getStyle().vAlign)
            {
                case TextStyle::VALIGN_BASELINE:
                    ty = -getLineSkip();
                    break;
                case TextStyle::VALIGN_TOP:
                case TextStyle::VALIGN_DEFAULT:
                    ty = 0;
                    break;
                case TextStyle::VALIGN_MIDDLE:
                    ty = height / 2 - ry / 2;
                    break;
                case TextStyle::VALIGN_BOTTOM:
                    ty = height - ry;
                    break;
            }
            return ty;
        }

        void Aligner::begin()
        {
            m_x = 0;
            _y = 0;

            width = int(width * _scale);
            height = int(height * _scale);

            bounds = Rect(_alignX(0), _alignY(0), 0, 0);
            nextLine();

        }

        void Aligner::end()
        {
            qint32 ry = _y;

            if (getStyle().multiline)
            {
                nextLine();
                _y -=  getLineSkip();
            }
            else
            {
                _alignLine(_line);
            }

            bounds.setY(_alignY(ry));
            bounds.setHeight(ry);
        }

        qint32 Aligner::getLineWidth() const
        {
            return _lineWidth;
        }

        qint32 Aligner::getLineSkip() const
        {
            return _lineSkip;
        }

        void Aligner::_alignLine(line& ln)
        {
            if (!ln.empty())
            {
                //calculate real text width
                qint32 rx = 0;
                for (qint32 i = 0; i < ln.size(); ++i)
                {
                    Symbol& s = *ln[i];
                    rx = std::max(s.x + s.gl.advance_x, rx);
                }

                qint32 tx = _alignX(rx);

                for (qint32 i = 0; i < ln.size(); ++i)
                {
                    Symbol& s = *ln[i];
                    s.x += tx;
                }

                _lineWidth = rx;

                bounds.setX(std::min(tx, bounds.getX()));
                bounds.setWidth(std::max(_lineWidth, bounds.getWidth()));
            }
        }

        void Aligner::_nextLine(line& ln)
        {
            _y += getLineSkip();
            _alignLine(ln);


            _lineWidth = 0;

            m_x = 0;
        }

        void Aligner::nextLine()
        {
            _nextLine(_line);
            _line.clear();
        }

        float Aligner::getScale() const
        {
            return _scale;
        }

        qint32 Aligner::putSymbol(Symbol& s)
        {
            if (_line.empty() && s.code == ' ')
            {
                return 0;
            }
            _line.push_back(&s);

            //optional remove?
            if (_line.size() == 1 && s.gl.offset_x < 0)
            {
                m_x -= s.gl.offset_x;
            }

            s.x = m_x + s.gl.offset_x;
            s.y = _y + s.gl.offset_y;
            m_x += s.gl.advance_x + getStyle().kerning;

            qint32 rx = s.x + s.gl.advance_x;
            _lineWidth = std::max(rx, _lineWidth);

            if (_lineWidth > width && getStyle().multiline && (width > 0) && _line.size() > 1)
            {
                qint32 lastWordPos = (int)_line.size() - 1;
                for (; lastWordPos > 0; --lastWordPos)
                {
                    if (_line[lastWordPos]->code == ' ' && _line[lastWordPos - 1]->code != ' ')
                    {
                        break;
                    }
                }

                if (!lastWordPos)
                {
                    if (style.breakLongWords)
                    {
                        lastWordPos = (int)_line.size() - 1;
                    }
                    else
                    {
                        return 0;
                    }
                }

                qint32 delta = (int)_line.size() - lastWordPos;
                line leftPart;
                leftPart.resize(delta + 1);
                leftPart = line(_line.begin() + lastWordPos, _line.end());
                _line.resize(lastWordPos);
                nextLine();
                for (qint32 i = 0; i < leftPart.size(); ++i)
                {
                    putSymbol(*leftPart[i]);
                }

                return 0;
            }
            assert(m_x > -1000);
            return 0;
        }
    }
}
