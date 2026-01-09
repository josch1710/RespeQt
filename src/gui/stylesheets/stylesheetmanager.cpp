#include "gui/stylesheets/stylesheetmanager.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QSharedPointer>

namespace gui {
    namespace stylesheets {

        StylesheetManager *StylesheetManager::instance = nullptr;

        StylesheetManager::StylesheetManager(QObject *parent) : QObject(parent)
        {
            // Stylesheets werden jetzt lazy initialisiert
        }

        StylesheetManager::~StylesheetManager()
        {
        }

        StylesheetManager *StylesheetManager::getInstance()
        {
            if (!instance)
            {
                instance = new StylesheetManager();
            }
            return instance;
        }
    

        QString StylesheetManager::getStylesheetContent(Style style)
        {
            switch (style)  // TODO make it polymorphic
            {
                case ClassicStyle:
                    if (!m_classicStylesheet) {
                        m_classicStylesheet = QSharedPointer<ClassicStylesheet>(new ClassicStylesheet());
                    }
                    return m_classicStylesheet->getContent();
                case ModernStyle:
                    if (!m_modernStylesheet) {
                        m_modernStylesheet = QSharedPointer<ModernStylesheet>(new ModernStylesheet());
                    }
                    return m_modernStylesheet->getContent();
                case DarkStyle:
                    if (!m_darkStylesheet) {
                        m_darkStylesheet = QSharedPointer<DarkStylesheet>(new DarkStylesheet());
                    }
                    return m_darkStylesheet->getContent();
                case CustomStyle:
                    if (!m_customStylesheet) {
                        m_customStylesheet = QSharedPointer<CustomStylesheet>(new CustomStylesheet());
                    }
                    return m_customStylesheet->getContent();
                default:
                    if (!m_classicStylesheet) {
                        m_classicStylesheet = QSharedPointer<ClassicStylesheet>(new ClassicStylesheet());
                    }
                    return m_classicStylesheet->getContent();
            }
        }
    
        QSharedPointer<Stylesheet> StylesheetManager::getStylesheet(Style style)
        {
            switch (style) // TODO make it polymorphic
            {
                case ClassicStyle:
                    if (!m_classicStylesheet) {
                        m_classicStylesheet = QSharedPointer<ClassicStylesheet>(new ClassicStylesheet());
                    }
                    return m_classicStylesheet;
                case ModernStyle:
                    if (!m_modernStylesheet) {
                        m_modernStylesheet = QSharedPointer<ModernStylesheet>(new ModernStylesheet());
                    }
                    return m_modernStylesheet;
                case DarkStyle:
                    if (!m_darkStylesheet) {
                        m_darkStylesheet = QSharedPointer<DarkStylesheet>(new DarkStylesheet());
                    }
                    return m_darkStylesheet;
                case CustomStyle:
                    if (!m_customStylesheet) {
                        m_customStylesheet = QSharedPointer<CustomStylesheet>(new CustomStylesheet());
                    }
                    return m_customStylesheet;
                default:
                    if (!m_classicStylesheet) {
                        m_classicStylesheet = QSharedPointer<ClassicStylesheet>(new ClassicStylesheet());
                    }
                    return m_classicStylesheet;
            }
        }

        void StylesheetManager::applyStyleToApplication(QApplication *app, Style style)
        {
            QString stylesheet = getStylesheetContent(style);
            app->setStyleSheet(stylesheet);
        }

        void StylesheetManager::setCustomStylesheet(const QString &stylesheet)
        {
            if (!m_customStylesheet) {
                m_customStylesheet = QSharedPointer<CustomStylesheet>(new CustomStylesheet());
            }
            m_customStylesheet->setContent(stylesheet);
        }
    } 
}  // namespace gui
