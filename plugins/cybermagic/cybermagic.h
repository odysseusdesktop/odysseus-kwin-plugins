#ifndef CYBERMAGIC_H
#define CYBERMAGIC_H

#include <kwineffects.h>
#include <QEasingCurve>

class Timeline {
public:
    enum Direction {
        Forward,
        Backward
    };

public:
    explicit Timeline(int duration = 1000, Direction direction = Forward)
        : m_duration(duration)
        , m_direction(direction)
    {
    }

    qreal progress() const {
        Q_ASSERT(m_duration > 0);
        auto t = static_cast<qreal>(m_elapsed) / m_duration;
        return m_direction == Backward ? 1 - t : t;
    }

    qreal value() const {
        return m_easingCurve.valueForProgress(progress());
    }

    qreal rprogress() const {
        Q_ASSERT(m_duration > 0);
        auto t = static_cast<qreal>(m_elapsed) / m_duration;
        return m_direction == Forward ? 1 - t : t;
    }

    qreal rvalue() const {
        return m_easingCurve.valueForProgress(rprogress());
    }

    void update(int delta) {
        Q_ASSERT(delta >= 0);
        if (m_finished) {
            return;
        }
        m_elapsed += delta;
        if (m_elapsed >= m_duration) {
            m_finished = true;
            m_elapsed = m_duration;
        }
    }

    int elapsed() const {
        return m_elapsed;
    }

    void setElapsed(int elapsed) {
        reset();
        update(elapsed);
    }

    int duration() const {
        return m_duration;
    }

    void setDuration(int duration) {
        Q_ASSERT(duration > 0);
        m_duration = duration;
    }

    Direction direction() const {
        return m_direction;
    }

    void setDirection(Direction direction) {
        if (m_direction == direction) {
            return;
        }
        if (m_elapsed > 0) {
            m_elapsed = m_duration - m_elapsed;
        }
        m_direction = direction;
    }

    void toggleDirection() {
        setDirection(m_direction == Forward ? Backward : Forward);
    }

    QEasingCurve easingCurve() const {
        return m_easingCurve;
    }

    void setEasingCurve(const QEasingCurve& easingCurve) {
        m_easingCurve = easingCurve;
    }

    void setEasingCurve(QEasingCurve::Type type) {
        m_easingCurve.setType(type);
    }

    bool done() const {
        return m_finished;
    }

    void reset() {
        m_elapsed = 0;
        m_finished = false;
    }

private:
    int m_duration;
    Direction m_direction;
    QEasingCurve m_easingCurve;

    int m_elapsed = 0;
    bool m_finished = false;
};

class CyberMagic : public KWin::Effect
{
    Q_OBJECT

public:
    CyberMagic();

    void reconfigure(ReconfigureFlags) override;
    void prePaintScreen(KWin::ScreenPrePaintData& data, int time);
    void prePaintWindow(KWin::EffectWindow* w, KWin::WindowPrePaintData& data, int time);
    void paintWindow(KWin::EffectWindow* w, int mask, QRegion region, KWin::WindowPaintData& data) override;
    void postPaintScreen() override;
    bool isActive() const override;

    int requestedEffectChainPosition() const override {
        return 50;
    }

    static bool supported();

public Q_SLOTS:
    void slotWindowDeleted(KWin::EffectWindow *w);
    void slotWindowMinimized(KWin::EffectWindow *w);
    void slotWindowUnminimized(KWin::EffectWindow *w);

private:
    QHash<const KWin::EffectWindow*, Timeline> m_animations;
    int m_duration;
    qreal m_scale;
};

#endif
