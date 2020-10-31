#include "stylehelper.h"

#include <QApplication>
#include <QPalette>

StyleHelper::StyleHelper()
{

}

void StyleHelper::setDarkStyle()
{
    QPalette palette;
    palette.setColor(QPalette::WindowText,QColor(212, 212, 212));
    palette.setColor(QPalette::Button,QColor(45, 45, 45));
    palette.setColor(QPalette::Light,QColor(60, 60, 60));
    palette.setColor(QPalette::Midlight,QColor(52, 52, 52));
    palette.setColor(QPalette::Dark,QColor(30, 30, 30));
    palette.setColor(QPalette::Mid,QColor(37, 37, 37));
    palette.setColor(QPalette::Active,QPalette::Text,QColor(212, 212, 212));
    palette.setColor(QPalette::Disabled,QPalette::Text,QColor(127, 127, 127));
    palette.setColor(QPalette::Inactive,QPalette::Text,QColor(212, 212, 212));
    palette.setColor(QPalette::BrightText,QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText,QColor(212, 212, 212));
    palette.setColor(QPalette::Base,QColor(60, 60, 60));
    palette.setColor(QPalette::Window,QColor(37, 37, 37));
    palette.setColor(QPalette::Shadow,QColor(0, 0, 0));
    palette.setColor(QPalette::Highlight,QColor(38, 79, 120));
    palette.setColor(QPalette::HighlightedText,QColor(240, 240, 240));
    palette.setColor(QPalette::Link,QColor(0, 0, 255));
    palette.setColor(QPalette::LinkVisited,QColor(255, 0, 255));
    palette.setColor(QPalette::AlternateBase,QColor(45, 45, 45));
    palette.setColor(QPalette::NoRole,QColor(0, 0, 0));
    palette.setColor(QPalette::ToolTipBase,QColor(255, 255, 220));
    palette.setColor(QPalette::ToolTipText,QColor(0, 0, 0));
    palette.setColor(QPalette::PlaceholderText,QColor(212, 212, 212, 128));
    qApp->setPalette(palette);
}

void StyleHelper::setLightStyle()
{
    QPalette palette;
    palette.setColor(QPalette::Active,QPalette::WindowText,QColor(0, 0, 0));
    palette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(120, 120, 120));
    palette.setColor(QPalette::Inactive,QPalette::WindowText,QColor(0, 0, 0));
    palette.setColor(QPalette::Button,QColor(240, 240, 240));
    palette.setColor(QPalette::Light,QColor(255, 255, 255));
    palette.setColor(QPalette::Active,QPalette::Midlight,QColor(227, 227, 227));
    palette.setColor(QPalette::Disabled,QPalette::Midlight,QColor(247, 247, 247));
    palette.setColor(QPalette::Inactive,QPalette::Midlight,QColor(227, 227, 227));
    palette.setColor(QPalette::Dark,QColor(160, 160, 160));
    palette.setColor(QPalette::Mid,QColor(160, 160, 160));
    palette.setColor(QPalette::Active,QPalette::Text,QColor(0, 0, 0));
    palette.setColor(QPalette::Disabled,QPalette::Text,QColor(120, 120, 120));
    palette.setColor(QPalette::Inactive,QPalette::Text,QColor(0, 0, 0));
    palette.setColor(QPalette::BrightText,QColor(255, 255, 255));
    palette.setColor(QPalette::Active,QPalette::ButtonText,QColor(0, 0, 0));
    palette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(120, 120, 120));
    palette.setColor(QPalette::Inactive,QPalette::ButtonText,QColor(0, 0, 0));
    palette.setColor(QPalette::Active,QPalette::Base,QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled,QPalette::Base,QColor(240, 240, 240));
    palette.setColor(QPalette::Inactive,QPalette::Base,QColor(255, 255, 255));
    palette.setColor(QPalette::Window,QColor(240, 240, 240));
    palette.setColor(QPalette::Active,QPalette::Shadow,QColor(105, 105, 105));
    palette.setColor(QPalette::Disabled,QPalette::Shadow,QColor(0, 0, 0));
    palette.setColor(QPalette::Inactive,QPalette::Shadow,QColor(105, 105, 105));
    palette.setColor(QPalette::Active,QPalette::Highlight,QColor(51, 153, 255));
    palette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(51, 153, 255));
    palette.setColor(QPalette::Inactive,QPalette::Highlight,QColor(240, 240, 240));
    palette.setColor(QPalette::Active,QPalette::HighlightedText,QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive,QPalette::HighlightedText,QColor(0, 0, 0));
    palette.setColor(QPalette::Link,QColor(0, 0, 255));
    palette.setColor(QPalette::LinkVisited,QColor(255, 0, 255));
    palette.setColor(QPalette::Active,QPalette::AlternateBase,QColor(233, 231, 227));
    palette.setColor(QPalette::Disabled,QPalette::AlternateBase,QColor(247, 247, 247));
    palette.setColor(QPalette::Inactive,QPalette::AlternateBase,QColor(233, 231, 227));
    palette.setColor(QPalette::NoRole,QColor(0, 0, 0));
    palette.setColor(QPalette::ToolTipBase,QColor(255, 255, 220));
    palette.setColor(QPalette::ToolTipText,QColor(0, 0, 0));
    palette.setColor(QPalette::PlaceholderText,QColor(0, 0, 0, 128));
    qApp->setPalette(palette);
}
