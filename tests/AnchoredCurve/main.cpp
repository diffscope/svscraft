#include <optional>

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QtEvents>

#include <SVSCraftCore/AnchoredCurve.h>

using namespace SVS;

class TestAnchorEditor : public QWidget {
public:
    explicit TestAnchorEditor(QWidget *parent = nullptr) {
        resize(1280, 400);
        setAttribute(Qt::WA_Hover);
    }

private:
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPen pen;
        auto primaryColor = QColor(112, 156, 255);
        pen.setColor(primaryColor);
        pen.setWidthF(1.5);

        if (curve.count() >= 2) {
            // Draw curve
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            auto knots = curve.anchors();
            auto startTick = knots.first().x;
            auto endTick = knots.last().x;

            QPainterPath curvePath;
            bool firstPoint = true;
            for (int i = startTick; i <= endTick; i++) {
                bool ok;
                auto value = curve.value(i, &ok);
                if (!ok) {
                    firstPoint = true;
                    continue;
                }
                if (firstPoint) {
                    curvePath.moveTo(i, value);
                    firstPoint = false;
                } else
                    curvePath.lineTo(i, value);
            }
            painter.drawPath(curvePath);
        }

        // Draw knots
        pen.setColor({255, 255, 255});
        pen.setWidthF(2);
        painter.setPen(pen);
        for (const auto &knot : curve.anchors()) {
            painter.setBrush(knot.interpolationMode == AnchoredCurve::Anchor::Linear ? Qt::red : knot.interpolationMode == AnchoredCurve::Anchor::Zero ? Qt::green : knot.interpolationMode == AnchoredCurve::Anchor::Pchip ? Qt::blue : Qt::gray);
            auto pos = QPointF(knot.x, knot.y);
            painter.drawEllipse(pos, 6, 6);
        }
    }
    void mousePressEvent(QMouseEvent *event) override {
        auto pos = event->position().toPoint();
        AnchoredCurve::Anchor::InterpolationMode interpolationMode = AnchoredCurve::Anchor::Pchip;
        if (event->modifiers() & Qt::ControlModifier) {
            interpolationMode = AnchoredCurve::Anchor::Linear;
        } else if (event->modifiers() & Qt::AltModifier) {
            interpolationMode = AnchoredCurve::Anchor::Zero;
        } else if (event->modifiers() & Qt::ShiftModifier) {
            interpolationMode = AnchoredCurve::Anchor::Break;
        }
        if (event->button() == Qt::LeftButton) {
            if (const auto node = findNode(pos); !node.isNull()) {
                currentEditNode = node;
            } else {
                auto newNode = AnchoredCurve::Anchor(pos.x(), pos.y(), interpolationMode);
                curve.addAnchor(newNode);
                currentEditNode = newNode;
            }
        } else if (event->button() == Qt::RightButton) {
            if (auto node = findNode(pos); !node.isNull()) {
                curve.removeAnchor(node.x);
                if (currentEditNode.x == node.x)
                    currentEditNode = {};
            }
        }
        update();
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        auto pos = event->position().toPoint();
        if (!currentEditNode.isNull()) {
            curve.removeAnchor(currentEditNode.x);
            currentEditNode.x = pos.x();
            currentEditNode.y = pos.y();
            curve.addAnchor(currentEditNode);
            update();
        }
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        currentEditNode = {};
        update();
    }
    bool event(QEvent *event) override {
        switch (event->type()) {
            case QEvent::HoverEnter:
            case QEvent::HoverMove:
            case QEvent::HoverLeave:
                handleHoverEvent(static_cast<QHoverEvent *>(event));
            break;
            default:
                break;
        }
        return QWidget::event(event);
    }
    void handleHoverEvent(QHoverEvent *event) {
        if (event->type() == QEvent::HoverMove) {
            auto node = findNode(event->position().toPoint());
            hoveredNode = node.x;
        } else if (event->type() == QEvent::HoverLeave) {
            hoveredNode = {};
        }
    }
    AnchoredCurve::Anchor findNode(const QPoint &position) const {
        return curve.anchor(position.x());
    }

    AnchoredCurve curve;
    std::optional<double> hoveredNode;
    AnchoredCurve::Anchor currentEditNode;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    TestAnchorEditor window;
    window.show();

    return a.exec();
}