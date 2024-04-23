/*
 * logdisplaydialog.cpp
 *
 * Copyright 2017 josch1710
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "logdisplaydialog.h"
#include "mainwindow.h"
#include "ui_logdisplaydialog.h"

#include <QMessageBox>
#include <QTranslator>

LogDisplayDialog::LogDisplayDialog(QWidget *parent) : QDialog(parent),
                                                      l_ui(new Ui::LogDisplayDialog)
{
    Qt::WindowFlags flags = windowFlags() & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    l_ui->setupUi(this);

    void (QComboBox::*diskFilterChanged)(int) = &QComboBox::currentIndexChanged;
    connect(l_ui->listByDisk, diskFilterChanged, this, &LogDisplayDialog::diskFilter);
    connect(l_ui->buttonBox, &QDialogButtonBox::clicked, this, &LogDisplayDialog::onClick);
}

LogDisplayDialog::~LogDisplayDialog()
{
    delete l_ui;
}
void LogDisplayDialog::onClick(QAbstractButton * /*button*/)
{
    this->close();
}

void LogDisplayDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            l_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void LogDisplayDialog::setLogText(QString logText)
{
    savedLog.clear();
    filter = "ALL";
    l_ui->listByDisk->setCurrentIndex(0);
    l_ui->textEdit->clear();
    l_ui->textEdit->ensureCursorVisible();
    if (!logText.isEmpty())
    {
        l_ui->textEdit->setHtml(logText);
        savedLog.append(logText);
    }
}


void LogDisplayDialog::addLogTextChange(QString logChange)
{
    if (filter == "ALL" || logChange.contains("[" + filter + "]"))
    {
        l_ui->textEdit->append(logChange);
    }
    savedLog.append(logChange);
    savedLog.append("<br>");
}

void LogDisplayDialog::diskFilter()
{
    QTextEdit searchResults;
    QTextDocument *search = l_ui->textEdit->document();
    QTextCursor cursor;
    filter = l_ui->listByDisk->currentText();
    searchResults.clear();
    if (filter != "ALL")
    {
        cursor.setPosition(0);
        cursor = search->find(filter, cursor, QTextDocument::FindWholeWords);
        while (!cursor.isNull())
        {
            int i = cursor.position();
            cursor.setPosition(i - filter.length() - 1);
            cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
            searchResults.append(cursor.selectedText());
            cursor.setPosition(cursor.position(), QTextCursor::MoveAnchor);
            cursor = search->find(filter, cursor, QTextDocument::FindWholeWords);
        }
        l_ui->textEdit->setHtml(searchResults.toHtml());
    }
    else
    {
        l_ui->textEdit->clear();
        l_ui->textEdit->setHtml(savedLog);
    }
}
