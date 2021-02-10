#include "cybermagic.h"
#include <KPluginFactory>

KWIN_EFFECT_FACTORY_ENABLED(CyberMagicFactory,
                            CyberMagic,
                            "cybermagic.json",
                            return true;)

CyberMagic::CyberMagic()
{
    bool magiclampLoaded = false;
    QMetaObject::invokeMethod(KWin::effects, "isEffectLoaded", Qt::DirectConnection, Q_RETURN_ARG(bool, magiclampLoaded), Q_ARG(QString, "magiclamp"));
    if (magiclampLoaded) {
        QMetaObject::invokeMethod(KWin::effects, "unloadEffect", Qt::DirectConnection, Q_ARG(QString, "magiclamp"));
    }

    reconfigure(ReconfigureAll);

    connect(KWin::effects, &KWin::EffectsHandler::windowDeleted, this, &CyberMagic::slotWindowDeleted);
    connect(KWin::effects, &KWin::EffectsHandler::windowMinimized, this, &CyberMagic::slotWindowMinimized);
    connect(KWin::effects, &KWin::EffectsHandler::windowUnminimized, this, &CyberMagic::slotWindowUnminimized);
}

void CyberMagic::reconfigure(ReconfigureFlags)
{
    m_duration = 300;
    m_scale = 1;
}

void CyberMagic::prePaintScreen(KWin::ScreenPrePaintData &data, int time)
{
    auto it = m_animations.begin();
    while (it != m_animations.end()) {
        Timeline& t = *it;
        t.update(time);
        if (t.done()) {
            it = m_animations.erase(it);
        } else {
            ++it;
        }
    }

    // We need to mark the screen windows as transformed. Otherwise the
    // whole screen won't be repainted, resulting in artefacts.
    if (!m_animations.isEmpty())
        data.mask |= PAINT_SCREEN_WITH_TRANSFORMED_WINDOWS;

    KWin::effects->prePaintScreen(data, time);
}

void CyberMagic::prePaintWindow(KWin::EffectWindow *w, KWin::WindowPrePaintData &data, int time)
{
    if (m_animations.contains(w)) {
        w->enablePainting(KWin::EffectWindow::PAINT_DISABLED_BY_MINIMIZE);
        data.setTransformed();
    }

    KWin::effects->prePaintWindow(w, data, time);
}

void CyberMagic::paintWindow(KWin::EffectWindow *w, int mask, QRegion region, KWin::WindowPaintData &data)
{
    const auto it = m_animations.constFind(w);
    if (it == m_animations.cend()) {
        KWin::effects->paintWindow(w, mask, region, data);
        return;
    }

    const QRect geometry(w->geometry());
    const QRect icon(w->iconGeometry());

    if (icon.isValid() && geometry.isValid()) {
        const qreal t = (*it).value();
        const qreal scaleProgress = interpolate(0, m_scale, t);

        const qreal targetScale = icon.width() > icon.height()
            ? static_cast<qreal>(icon.width()) / geometry.width()
            : static_cast<qreal>(icon.height()) / geometry.height();

        data.setXScale(interpolate(1, targetScale, scaleProgress));
        data.setYScale(interpolate(1, targetScale, scaleProgress));
        data.setXTranslation(interpolate(0, icon.x() - geometry.x(), scaleProgress));
        data.setYTranslation(interpolate(0, icon.y() - geometry.y(), scaleProgress));
    }

    KWin::effects->paintWindow(w, mask, region, data);
}

void CyberMagic::postPaintScreen()
{
    if (!m_animations.isEmpty()) {
        KWin::effects->addRepaintFull();
    }

    KWin::effects->postPaintScreen();
}

bool CyberMagic::isActive() const
{
    return !m_animations.isEmpty();
}

bool CyberMagic::supported()
{
    return KWin::effects->isOpenGLCompositing() && KWin::effects->animationsSupported();
}

void CyberMagic::slotWindowDeleted(KWin::EffectWindow *w)
{
    m_animations.remove(w);
}

void CyberMagic::slotWindowMinimized(KWin::EffectWindow *w)
{
    if (KWin::effects->activeFullScreenEffect() != nullptr) {
        return;
    }

    Timeline& t = m_animations[w];
    t.setDuration(m_duration);
    t.setDirection(Timeline::Forward);
    t.setEasingCurve(QEasingCurve::InOutQuad);
}

void CyberMagic::slotWindowUnminimized(KWin::EffectWindow *w)
{
    if (KWin::effects->activeFullScreenEffect() != nullptr) {
        return;
    }

    Timeline& t = m_animations[w];
    t.setDuration(m_duration);
    t.setDirection(Timeline::Backward);
    t.setEasingCurve(QEasingCurve::InOutQuad);
}

#include "cybermagic.moc"
