#ifndef SVSCRAFT_COLORBLENDER_H
#define SVSCRAFT_COLORBLENDER_H

#include <cmath>

#include <QColor>

#include <SVSCraftGui/svscraftguiglobal.h>

namespace SVS {

    struct ColorBlender {
        struct Normal {
            inline constexpr int operator()(int cb, int cs) const {
                return cs;
            }
        };
        struct Multiply {
            inline constexpr int operator()(int cb, int cs) const {
                return pn(cb * cs);
            }
        };
        struct Screen {
            inline constexpr int operator()(int cb, int cs) const {
                return cb + cs - pn(cb * cs);
            }
        };
        struct Overlay {
            inline constexpr int operator()(int cb, int cs) const {
                if (cb & 0x80) {
                    cb = cb << 1 & ~0x80;
                    return cb + cs - pn(cb * cs);
                } else {
                    return pn((cb << 1) * cs);
                }

            }
        };
        struct Darken {
            inline constexpr int operator()(int cb, int cs) const {
                return qMin(cb, cs);
            }
        };
        struct Lighten {
            inline constexpr int operator()(int cb, int cs) const {
                return qMax(cb, cs);
            }
        };
        struct ColorDodge {
            inline constexpr int operator()(int cb, int cs) const {
                if (cb == 0)
                    return 0;
                if (cs == 0xff)
                    return 0xff;
                return qMin(0xff, static_cast<int>(cb / (255.0f - cs) * 255.0f));
            }
        };
        struct ColorBurn {
            inline constexpr int operator()(int cb, int cs) const {
                if (cs == 0)
                    return 0;
                if (cb == 0xff)
                    return 0xff;
                return 0xff - qMin(0xff, static_cast<int>((255.0f - cb) * 255.0f / cs));
            }
        };
        struct HardLight {
            inline constexpr int operator()(int cb, int cs) const {
                if (cs & 0x80) {
                    cs = cs << 1 & ~0x80;
                    return cb + cs - pn(cb * cs);
                } else {
                    return pn(cb * (cs << 1));
                }
            }
        };
        struct SoftLight {
            inline constexpr int operator()(int cb, int cs) const {
                if (cs & 0x80) {
                    float cbn = cb / 255.0f;
                    int d = static_cast<int>((cbn <= 0.25f ? ((16 * cbn - 12) * cbn + 4) * cbn : std::sqrt(cbn)) * 255.0f);
                    return cb + pn((cs << 1 & ~0x80) * (d - cb));
                } else {
                    return cb - pn(pn((0xff - (cs << 1)) * cb) * (0xff - cb));
                }
            }
        };
        struct Difference {
            inline constexpr int operator()(int cb, int cs) const {
                return qAbs(cb - cs);
            }
        };
        struct Exclusion {
            inline constexpr int operator()(int cb, int cs) const {
                return cb + cs - pn(cb * cs);
            }
        };


        inline constexpr static QRgb compositePremultiplied(QRgb source, QRgb backdrop) {
            return qRgba(
                qRed(source) + pn(qRed(backdrop) * (0xff - qAlpha(source))),
                qGreen(source) + pn(qGreen(backdrop) * (0xff - qAlpha(source))),
                qBlue(source) + pn(qBlue(backdrop) * (0xff - qAlpha(source))),
                qAlpha(source) + pn(qAlpha(backdrop) * (0xff - qAlpha(source)))
            );
        }

        inline static QRgb composite(QRgb source, QRgb backdrop) {
            return qUnpremultiply(compositePremultiplied(qPremultiply(source), qPremultiply(backdrop)));
        }

        template <typename Blender>
        inline static QRgb blend(QRgb source, QRgb backdrop) {
            Blender b;
            source = qRgba(
                pn((0xff - qAlpha(backdrop)) * qRed(source)) + pn(qAlpha(backdrop) * b(qRed(backdrop), qRed(source))),
                pn((0xff - qAlpha(backdrop)) * qGreen(source)) + pn(qAlpha(backdrop) * b(qGreen(backdrop), qGreen(source))),
                pn((0xff - qAlpha(backdrop)) * qBlue(source)) + pn(qAlpha(backdrop) * b(qBlue(backdrop), qBlue(source))),
                qAlpha(source)
            );
            return composite(source, backdrop);
        }

    private:
        inline constexpr static int pn(int x) {
            return (x + (x >> 8) + 0x80) >> 8;
        }

    };

}

#endif //COLORBLENDER_H
