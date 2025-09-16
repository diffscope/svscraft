/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#include "WaveformThumbnail_p.h"
#include "WaveformThumbnail_p_p.h"

#include <cmath>
#include <limits>

#include <QSGNode>
#include <QSGGeometry>
#include <QSGFlatColorMaterial>

namespace SVS {
    
    WaveformThumbnail::WaveformThumbnail(QQuickItem *parent) : QQuickItem(parent), d_ptr(new WaveformThumbnailPrivate) {
        Q_D(WaveformThumbnail);
        d->q_ptr = this;
        setFlag(ItemHasContents, true);
        connect(this, &QQuickItem::xChanged, &QQuickItem::update);
        connect(this, &QQuickItem::yChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::colorChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::rmsColorChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformOffsetChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformLengthChanged, &QQuickItem::update);
        connect(this, &WaveformThumbnail::waveformMipmapChanged, &QQuickItem::update);

    }
    WaveformThumbnail::~WaveformThumbnail() = default;
    QColor WaveformThumbnail::color() const {
        Q_D(const WaveformThumbnail);
        return d->color;
    }
    void WaveformThumbnail::setColor(const QColor &color) {
        Q_D(WaveformThumbnail);
        if (d->color != color) {
            d->color = color;
            emit colorChanged();
        }
    }
    QColor WaveformThumbnail::rmsColor() const {
        Q_D(const WaveformThumbnail);
        return d->rmsColor;
    }
    void WaveformThumbnail::setRmsColor(const QColor &rmsColor) {
        Q_D(WaveformThumbnail);
        if (d->rmsColor != rmsColor) {
            d->rmsColor = rmsColor;
            emit rmsColorChanged();
        }
    }
    double WaveformThumbnail::waveformOffset() const {
        Q_D(const WaveformThumbnail);
        return d->waveformOffset;
    }
    void WaveformThumbnail::setWaveformOffset(double waveformOffset) {
        Q_D(WaveformThumbnail);
        if (d->waveformOffset != waveformOffset) {
            d->waveformOffset = waveformOffset;
            emit waveformOffsetChanged();
        }
    }
    double WaveformThumbnail::waveformLength() const {
        Q_D(const WaveformThumbnail);
        return d->waveformLength;
    }
    void WaveformThumbnail::setWaveformLength(double waveformLength) {
        Q_D(WaveformThumbnail);
        if (d->waveformLength != waveformLength) {
            d->waveformLength = waveformLength;
            emit waveformLengthChanged();
        }
    }
    WaveformMipmap WaveformThumbnail::waveformMipmap() const {
        Q_D(const WaveformThumbnail);
        return d->waveformMipmap;
    }
    void WaveformThumbnail::setWaveformMipmap(const WaveformMipmap &waveformMipmap) {
        Q_D(WaveformThumbnail);
        d->waveformMipmap = waveformMipmap;
        d->waveformMipmapUpdatePending = true;
        emit waveformMipmapChanged();
    }

    class WaveformThumbnailSGNode : public QSGNode {
    public:
        double m_waveformOffset = -1;
        double m_waveformLength = -1;
        double m_width = 0;
        bool m_isMipmap = false;
    };

    static constexpr int INTERPOLATE_WINDOW = 16;
    static constexpr int INTERPOLATE_MAX = 16;
    static constexpr double SINC[] =
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.004122767017195896, 0.0044159415606408615, -0.004754004072460345, 0.005148118399710856, -0.00561348503471307, 0.006171346901516748, -0.00685232311133929, 0.007702223652280601, -0.008792803992426533, 0.01024316341385772, -0.012266504335113573, 0.015285951556064617, -0.02027728267641226, 0.030108692458915322, -0.05844628536142389, 0.9935868511442058, 0.06623912340961384, -0.03205118874658736, 0.02114014576902562, -0.015771219859431824, 0.01257704874866083, -0.010458808959412695, 0.00895123289319105, -0.007823518512946515, 0.006948159798211244, -0.0062489739065673495, 0.005677639149395583, -0.005202025398660784, 0.004799936478957625, -0.004455546417687045, 0.004157267159599286, -0.00389641902409495, -0.008053680647970525, 0.008623852729242704, -0.009280908175280324, 0.010046343901076558, -0.01094938604948803, 0.01203080689388181, -0.01334925148499215, 0.014992236283145034, -0.017096409796568904, 0.01988766037560057, -0.023768179473278736, 0.029530162375891896, -0.03897981433617731, 0.05732325637673136, -0.10827726204493694, 0.9744953584044327, 0.13921362262920473, -0.06496635722696228, 0.04236936340888846, -0.031435334142078546, 0.024987060471908594, -0.020733943795838962, 0.01771809742553512, -0.015468180292133838, 0.013725286738090594, -0.01233538428360041, 0.011201096073614266, -0.0102578458779414, 0.009461119984509143, -0.008779237463103004, 0.008189036625247413, -0.007673191798460111, -0.011644016305486194, 0.012464739921731853, -0.01340993347029927, 0.01451023570375965, -0.015807240012475684, 0.0173588709339456, -0.01924827185192618, 0.021599205818573657, -0.02460431271507086, 0.02858076729528434, -0.034090312797989755, 0.042231283018405126, -0.05548031298496361, 0.08084274177808985, -0.14892084011753395, 0.9431653207443821, 0.21765353555639586, -0.0975688262839016, 0.06287768804962549, -0.04638517970874012, 0.0367467008082227, -0.030424687765947915, 0.02595867855259777, -0.02263596769786526, 0.02006734724988055, -0.018022267275370433, 0.016355467989787046, -0.014970878107053671, 0.013802419327966633, -0.012803149150376222, 0.011938801528409962, -0.01118377850685038, -0.014759283871427954, 0.015795023090475476, -0.016987100304851045, 0.01837380237055311, -0.020007029247935678, 0.021958934540417138, -0.02433287340965151, 0.027282312610821393, -0.031045390212314006, 0.03601265264628425, -0.04287220553129077, 0.0529597833033592, -0.06925510124285435, 0.10003514623967844, -0.18006326323142122, 0.9003163161571061, 0.3001054387190354, -0.12861661659387233, 0.0818469378324643, -0.060021087743807156, 0.04738506927142671, -0.03914418765900468, 0.033345048746559545, -0.029042461811519604, 0.02572332331877451, -0.023085033747618156, 0.020937588747839724, -0.019155666301215, 0.017653261101119772, -0.01636938756649282, 0.015259598578934044, -0.01429073517709691, -0.017284244744966756, 0.018491877565575746, -0.019880938791158948, 0.021495634327496682, -0.023395800897883186, 0.025664484621314227, -0.02842040243299904, 0.031839398214412465, -0.03619350395313554, 0.04192712834175107, -0.04981929367666892, 0.06137159365966462, -0.07989886721729923, 0.11444972871667189, -0.20164952202461237, 0.8469279925033719, 0.38496726931971464, -0.15683851713025412, 0.0984799991282991, -0.07177355868672647, 0.05646186616689149, -0.04653450508260289, 0.039576074416045444, -0.03442796717493385, 0.03046503570156017, -0.027320257822689445, 0.024763976388987512, -0.02264513348939494, 0.020860295381856478, -0.019336255536606645, 0.018019744521348365, -0.016871075547875918, -0.01912715374576918, 0.020457738354170543, -0.021987288885323454, 0.023764039502319322, -0.025853185832193513, 0.028345059165417023, -0.03136853214306147, 0.03511402851835239, -0.03987525272423068, 0.046130194328031573, -0.05471255606347931, 0.06721828316370317, -0.08713481150850409, 0.12382315319629532, -0.21387635552087372, 0.7842133035765372, 0.4705279821459223, -0.18097230082535468, 0.1120304719395053, -0.08112551416309005, 0.06358486245215167, -0.05228088690510248, 0.04438943227791721, -0.03856786738901003, 0.03409623059028423, -0.030553765074410547, 0.02767811659681897, -0.02529720334117867, 0.02329346446266943, -0.02158385239201483, 0.020108033425039425, -0.01882111928583693, -0.020223070508559635, 0.021623802664996677, -0.02323301588657781, 0.025100997063388104, -0.027295619757454807, 0.02991076895577385, -0.03308012195771013, 0.03700072900454985, -0.04197561693793471, 0.04849610112246826, -0.05741492431740495, 0.07035349881146806, -0.09081997119298604, 0.12807944655421108, -0.21717819198322746, 0.7135854879448903, 0.5550109350682481, -0.19980393662456927, 0.1218316686735179, -0.08763330553709182, 0.06842600569334567, -0.05612470129903631, 0.04757236586299268, -0.041281805087720926, 0.03646057237674622, -0.03264770206283812, 0.029556795358664095, -0.02700053197629317, 0.02485123589857827, -0.023018886707899706, 0.02143819062495379, -0.02006063620728609, -0.02053612168927682, 0.021952405943709702, -0.0235785100876882, 0.025464790894703253, -0.027679120537720932, 0.03031522725559911, -0.033506303808820075, 0.03744822190397537, -0.04244131815783876, 0.048970751720583176, -0.057874524760689224, 0.0707355302630646, -0.09094568176679733, 0.12732395447351627, -0.2122065907891938, 0.6366197723675814, 0.6366197723675814, -0.2122065907891938, 0.12732395447351627, -0.09094568176679733, 0.0707355302630646, -0.057874524760689224, 0.048970751720583176, -0.04244131815783876, 0.03744822190397537, -0.033506303808820075, 0.03031522725559911, -0.027679120537720932, 0.025464790894703253, -0.0235785100876882, 0.021952405943709702, -0.02053612168927682, -0.02006063620728609, 0.02143819062495379, -0.023018886707899706, 0.02485123589857827, -0.02700053197629317, 0.029556795358664095, -0.03264770206283812, 0.03646057237674622, -0.041281805087720926, 0.04757236586299268, -0.05612470129903631, 0.06842600569334567, -0.08763330553709182, 0.1218316686735179, -0.19980393662456927, 0.5550109350682481, 0.7135854879448903, -0.21717819198322746, 0.12807944655421108, -0.09081997119298604, 0.07035349881146806, -0.05741492431740495, 0.04849610112246826, -0.04197561693793471, 0.03700072900454985, -0.03308012195771013, 0.02991076895577385, -0.027295619757454807, 0.025100997063388104, -0.02323301588657781, 0.021623802664996677, -0.020223070508559635, -0.01882111928583693, 0.020108033425039425, -0.02158385239201483, 0.02329346446266943, -0.02529720334117867, 0.02767811659681897, -0.030553765074410547, 0.03409623059028423, -0.03856786738901003, 0.04438943227791721, -0.05228088690510248, 0.06358486245215167, -0.08112551416309005, 0.1120304719395053, -0.18097230082535468, 0.4705279821459223, 0.7842133035765372, -0.21387635552087372, 0.12382315319629532, -0.08713481150850409, 0.06721828316370317, -0.05471255606347931, 0.046130194328031573, -0.03987525272423068, 0.03511402851835239, -0.03136853214306147, 0.028345059165417023, -0.025853185832193513, 0.023764039502319322, -0.021987288885323454, 0.020457738354170543, -0.01912715374576918, -0.016871075547875918, 0.018019744521348365, -0.019336255536606645, 0.020860295381856478, -0.02264513348939494, 0.024763976388987512, -0.027320257822689445, 0.03046503570156017, -0.03442796717493385, 0.039576074416045444, -0.04653450508260289, 0.05646186616689149, -0.07177355868672647, 0.0984799991282991, -0.15683851713025412, 0.38496726931971464, 0.8469279925033719, -0.20164952202461237, 0.11444972871667189, -0.07989886721729923, 0.06137159365966462, -0.04981929367666892, 0.04192712834175107, -0.03619350395313554, 0.031839398214412465, -0.02842040243299904, 0.025664484621314227, -0.023395800897883186, 0.021495634327496682, -0.019880938791158948, 0.018491877565575746, -0.017284244744966756, -0.01429073517709691, 0.015259598578934044, -0.01636938756649282, 0.017653261101119772, -0.019155666301215, 0.020937588747839724, -0.023085033747618156, 0.02572332331877451, -0.029042461811519604, 0.033345048746559545, -0.03914418765900468, 0.04738506927142671, -0.060021087743807156, 0.0818469378324643, -0.12861661659387233, 0.3001054387190354, 0.9003163161571061, -0.18006326323142122, 0.10003514623967844, -0.06925510124285435, 0.0529597833033592, -0.04287220553129077, 0.03601265264628425, -0.031045390212314006, 0.027282312610821393, -0.02433287340965151, 0.021958934540417138, -0.020007029247935678, 0.01837380237055311, -0.016987100304851045, 0.015795023090475476, -0.014759283871427954, -0.01118377850685038, 0.011938801528409962, -0.012803149150376222, 0.013802419327966633, -0.014970878107053671, 0.016355467989787046, -0.018022267275370433, 0.02006734724988055, -0.02263596769786526, 0.02595867855259777, -0.030424687765947915, 0.0367467008082227, -0.04638517970874012, 0.06287768804962549, -0.0975688262839016, 0.21765353555639586, 0.9431653207443821, -0.14892084011753395, 0.08084274177808985, -0.05548031298496361, 0.042231283018405126, -0.034090312797989755, 0.02858076729528434, -0.02460431271507086, 0.021599205818573657, -0.01924827185192618, 0.0173588709339456, -0.015807240012475684, 0.01451023570375965, -0.01340993347029927, 0.012464739921731853, -0.011644016305486194, -0.007673191798460111, 0.008189036625247413, -0.008779237463103004, 0.009461119984509143, -0.0102578458779414, 0.011201096073614266, -0.01233538428360041, 0.013725286738090594, -0.015468180292133838, 0.01771809742553512, -0.020733943795838962, 0.024987060471908594, -0.031435334142078546, 0.04236936340888846, -0.06496635722696228, 0.13921362262920473, 0.9744953584044327, -0.10827726204493694, 0.05732325637673136, -0.03897981433617731, 0.029530162375891896, -0.023768179473278736, 0.01988766037560057, -0.017096409796568904, 0.014992236283145034, -0.01334925148499215, 0.01203080689388181, -0.01094938604948803, 0.010046343901076558, -0.009280908175280324, 0.008623852729242704, -0.008053680647970525, -0.00389641902409495, 0.004157267159599286, -0.004455546417687045, 0.004799936478957625, -0.005202025398660784, 0.005677639149395583, -0.0062489739065673495, 0.006948159798211244, -0.007823518512946515, 0.00895123289319105, -0.010458808959412695, 0.01257704874866083, -0.015771219859431824, 0.02114014576902562, -0.03205118874658736, 0.06623912340961384, 0.9935868511442058, -0.05844628536142389, 0.030108692458915322, -0.02027728267641226, 0.015285951556064617, -0.012266504335113573, 0.01024316341385772, -0.008792803992426533, 0.007702223652280601, -0.00685232311133929, 0.006171346901516748, -0.00561348503471307, 0.005148118399710856, -0.004754004072460345, 0.0044159415606408615, -0.004122767017195896};
    static constexpr double sinc(int j, int k) {
        return SINC[k * (INTERPOLATE_WINDOW * 2) + (j + INTERPOLATE_WINDOW - 1)];
    }

    template <typename T>
    static inline double interpolate(T *a, qsizetype size, double x) {
        qsizetype i = std::floor(x);
        int k = static_cast<int>((x - i) * 16);
        if (k == 0) {
            return 1.0 * a[i] / std::numeric_limits<T>::max();
        }
        double ret = 0;
        for (int j = -INTERPOLATE_WINDOW + 1; j <= INTERPOLATE_WINDOW; j++) {
            qsizetype idx = i + j;
            if (idx >= 0 && idx < size) {
                ret += a[idx] * sinc(j, k);
            }
        }
        ret = qMax(-1.0, ret / std::numeric_limits<T>::max());
        return ret;
    }

    QSGNode *WaveformThumbnail::updatePaintNode(QSGNode *node_, UpdatePaintNodeData *update_paint_node_data) {
        Q_D(const WaveformThumbnail);
        if (!d->waveformMipmap.isValid()) {
            delete node_;
            return nullptr;
        }
        auto node = static_cast<WaveformThumbnailSGNode *>(node_);
        if (!node) {
            node = new WaveformThumbnailSGNode();

            auto mipmapNode = new QSGTransformNode;
            mipmapNode->setFlag(QSGNode::OwnedByParent);
            node->appendChildNode(mipmapNode);

            auto peakNode = new QSGGeometryNode;
            peakNode->setFlag(QSGNode::OwnsGeometry);
            peakNode->setFlag(QSGNode::OwnsMaterial);
            peakNode->setFlag(QSGNode::OwnedByParent);
            auto peakNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            peakNodeGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
            peakNode->setGeometry(peakNodeGeometry);
            auto peakNodeMaterial = new QSGFlatColorMaterial;
            peakNode->setMaterial(peakNodeMaterial);
            mipmapNode->appendChildNode(peakNode);

            auto peakLineNode = new QSGGeometryNode;
            peakLineNode->setFlag(QSGNode::OwnsGeometry);
            peakLineNode->setFlag(QSGNode::OwnedByParent);
            auto peakLineNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            peakLineNodeGeometry->setDrawingMode(QSGGeometry::DrawLines);
            peakLineNode->setGeometry(peakLineNodeGeometry);
            peakLineNode->setMaterial(peakNodeMaterial);
            mipmapNode->appendChildNode(peakLineNode);

            auto rmsNode = new QSGGeometryNode;
            rmsNode->setFlag(QSGNode::OwnsGeometry);
            rmsNode->setFlag(QSGNode::OwnsMaterial);
            rmsNode->setFlag(QSGNode::OwnedByParent);
            auto rmsNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            rmsNodeGeometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
            rmsNode->setGeometry(rmsNodeGeometry);
            auto rmsNodeMaterial = new QSGFlatColorMaterial;
            rmsNode->setMaterial(rmsNodeMaterial);
            mipmapNode->appendChildNode(rmsNode);

            auto lineGraphNode = new QSGGeometryNode;
            lineGraphNode->setFlag(QSGNode::OwnsGeometry);
            lineGraphNode->setFlag(QSGNode::OwnedByParent);
            auto lineGraphNodeGeometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            lineGraphNodeGeometry->setDrawingMode(QSGGeometry::DrawLineStrip);
            lineGraphNode->setGeometry(lineGraphNodeGeometry);
            lineGraphNode->setMaterial(peakNodeMaterial);
            mipmapNode->appendChildNode(lineGraphNode);
        }
        auto mipmapNode = static_cast<QSGTransformNode *>(node->childAtIndex(0));
        auto peakNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(0));
        auto peakNodeGeometry = peakNode->geometry();
        auto peakLineNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(1));
        auto peakLineNodeGeometry = peakLineNode->geometry();
        auto peakNodeMaterial = static_cast<QSGFlatColorMaterial *>(peakNode->material());
        auto rmsNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(2));
        auto rmsNodeGeometry = rmsNode->geometry();
        auto rmsNodeMaterial = static_cast<QSGFlatColorMaterial *>(rmsNode->material());
        auto lineGraphNode = static_cast<QSGGeometryNode *>(mipmapNode->childAtIndex(3));
        auto lineGraphNodeGeometry = lineGraphNode->geometry();

        if (d->color != peakNodeMaterial->color()) {
            peakNodeMaterial->setColor(d->color);
            peakNode->markDirty(QSGNode::DirtyMaterial);
            lineGraphNode->markDirty(QSGNode::DirtyMaterial);
        }

        if (d->rmsColor != rmsNodeMaterial->color()) {
            rmsNodeMaterial->setColor(d->rmsColor);
            rmsNode->markDirty(QSGNode::DirtyMaterial);
        }

        QMatrix4x4 matrix;
        matrix.scale(1, height());
        mipmapNode->setMatrix(matrix);

        if (!d->waveformMipmapUpdatePending && d->waveformOffset == node->m_waveformOffset && d->waveformLength == node->m_waveformLength && width() == node->m_width)
            return node;

        auto waveformLengthPerPixel = d->waveformLength / width();
        bool isMipmap = d->waveformMipmap.level() == WaveformMipmap::Downscale || waveformLengthPerPixel >= 4;
        int interpolateLevel = qBound(1.0, std::pow(2, std::floor(2 - std::log2(waveformLengthPerPixel))), 1.0 * INTERPOLATE_MAX);
        int pointCount = qMax(0.0, std::floor((d->waveformOffset + d->waveformLength) * interpolateLevel) - std::ceil(d->waveformOffset * interpolateLevel) + 1.0);

        if (isMipmap) {
            if (isMipmap != node->m_isMipmap || width() != node->m_width) {
                peakNodeGeometry->allocate(std::ceil(width()) * 2);
                rmsNodeGeometry->allocate(std::ceil(width()) * 2);
            }
            lineGraphNodeGeometry->allocate(0);
        } else {
            if (isMipmap != node->m_isMipmap || width() != node->m_width || d->waveformOffset != node->m_waveformOffset || d->waveformLength != node->m_waveformLength) {
                lineGraphNodeGeometry->allocate(pointCount);
            }
            peakNodeGeometry->allocate(0);
            peakLineNodeGeometry->allocate(0);
            rmsNodeGeometry->allocate(0);
        }

        node->m_width = width();
        node->m_waveformOffset = d->waveformOffset;
        node->m_waveformLength = d->waveformLength;
        node->m_isMipmap = isMipmap;
        d->waveformMipmapUpdatePending = false;

        if (isMipmap) {
            QList<QPair<float, float>> zeroDynamicPeaks;
            for (int i = 0; i < std::ceil(node->m_width); i++) {
                auto waveformBlockOffset = node->m_waveformOffset + i * waveformLengthPerPixel;
                auto waveformBlockLength = qMax(2.0, waveformLengthPerPixel);
                auto peak = d->waveformMipmap.peak(std::round(waveformBlockOffset), std::round(waveformBlockLength));
                auto normalizedPeak = qMakePair(
                    qMax(-1.0, 1.0 * peak.first / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0)),
                    qMax(-1.0, 1.0 * peak.second / (d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? 127.0 : 32767.0))
                );
                if (peak.first == peak.second) {
                    zeroDynamicPeaks.emplace_back(i, (1.0 - normalizedPeak.first) * 0.5);
                }
                peakNodeGeometry->vertexDataAsPoint2D()[i * 2].set(i, (1.0 - normalizedPeak.first) * 0.5);
                peakNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(i, (1.0 - normalizedPeak.second) * 0.5);
                if (d->waveformMipmap.useRms() && waveformLengthPerPixel > 16) {
                    auto rms = waveformBlockLength < 0.5 ? 0 : d->waveformMipmap.rms(std::round(waveformBlockOffset), std::round(waveformBlockLength));
                    rmsNodeGeometry->vertexDataAsPoint2D()[i * 2].set(i, (1.0 - rms) * 0.5);
                    rmsNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(i, (1.0 + rms) * 0.5);
                } else {
                    rmsNodeGeometry->vertexDataAsPoint2D()[i * 2].set(0, 0);
                    rmsNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(0, 0);
                }

            }
            peakLineNodeGeometry->allocate(zeroDynamicPeaks.size() * 2);
            for (int i = 0; i < zeroDynamicPeaks.size(); i++) {
                auto p = zeroDynamicPeaks.at(i);
                peakLineNodeGeometry->vertexDataAsPoint2D()[i * 2].set(p.first, p.second);
                peakLineNodeGeometry->vertexDataAsPoint2D()[i * 2 + 1].set(p.first + 1, p.second);
            }
        } else {
            for (qint64 i = std::ceil(d->waveformOffset * interpolateLevel), j = 0; i <= std::floor((d->waveformOffset + d->waveformLength) * interpolateLevel); i++, j++) {
                double index = 1.0 * i / interpolateLevel;
                auto value = d->waveformMipmap.sampleType() == WaveformMipmap::Int8 ? interpolate(d->waveformMipmap.originalDataAsInt8(), d->waveformMipmap.size(), index) : interpolate(d->waveformMipmap.originalDataAsInt16(), d->waveformMipmap.size(), index);
                double pointX = (index - d->waveformOffset) / d->waveformLength * width();
                double pointY = (1.0 - value) * 0.5;
                lineGraphNodeGeometry->vertexDataAsPoint2D()[j].set(pointX, pointY);
            }
        }
        peakNode->markDirty(QSGNode::DirtyGeometry);
        peakLineNode->markDirty(QSGNode::DirtyGeometry);
        rmsNode->markDirty(QSGNode::DirtyGeometry);
        lineGraphNode->markDirty(QSGNode::DirtyGeometry);

        return node;

    }
}

#include "moc_WaveformThumbnail_p.cpp"
