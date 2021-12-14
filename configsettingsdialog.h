/*
 * Copyright (C) 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     wangchunlin<wangchunlin@uniontech.com>
 *
 * Maintainer: wangchunlin<wangchunlin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CONFIGSETTINGSDIALOG_H
#define CONFIGSETTINGSDIALOG_H

#include <DLabel>
#include <DPushButton>
#include <DFrame>
#include <DSettingsDialog>
#include <QScopedPointer>

DCORE_BEGIN_NAMESPACE
class QSettingBackend;
class DSettings;
DCORE_END_NAMESPACE

class ConfigSettingsDialog : public Dtk::Widget::DSettingsDialog
{
    Q_OBJECT
public:
    explicit ConfigSettingsDialog(QWidget *parent = nullptr);
    ~ConfigSettingsDialog() override;

    void setScreenSaverName(const QString &name);
    QString screenSaverName() const;

private:
    QString configLocationPath();
    QString configGeneralPath();
    QString jsonPath();

public:
    static QPair<QWidget*, QWidget*> createSelectWorkPathWidget(QObject* opt);

private:
    QScopedPointer<Dtk::Core::QSettingBackend> m_backend;
    QScopedPointer<Dtk::Core::DSettings> m_settings;

    QString m_screenSaverName;

public:
    static Dtk::Widget::DFrame *m_frame;
    static Dtk::Widget::DLabel *m_pathLabel;
    static Dtk::Widget::DPushButton *m_btn;
};

#endif // CONFIGSETTINGSDIALOG_H
