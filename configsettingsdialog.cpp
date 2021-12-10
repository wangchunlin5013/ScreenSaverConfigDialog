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
#include "configsettingsdialog.h"

#include <qsettingbackend.h>
#include <DSettings>
#include <DSettingsOption>
#include <DTitlebar>
#include <DSettingsWidgetFactory>
#include <DFileDialog>

#include <QStandardPaths>
#include <DApplication>
#include <QDebug>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

Dtk::Widget::DFrame *ConfigSettingsDialog::m_frame = nullptr;
Dtk::Widget::DLabel *ConfigSettingsDialog::m_pathLabel = nullptr;
Dtk::Widget::DPushButton *ConfigSettingsDialog::m_btn = nullptr;

ConfigSettingsDialog::ConfigSettingsDialog(QWidget *parent)
    : DSettingsDialog(parent)
{
    // DSettingsDialog强制附带titleBar，需要隐藏
    DTitlebar *bar = this->findChild<DTitlebar *>();
    if (bar) {
        bar->setVisible(false);
        bar->setEmbedMode(false);
        bar->setFixedHeight(0);
        bar->setFixedWidth(0);
        layout()->removeWidget(bar);
    }

    widgetFactory()->registerWidget("selectDir", &ConfigSettingsDialog::createSelectWorkPathWidget);
}

ConfigSettingsDialog::~ConfigSettingsDialog()
{

}

void ConfigSettingsDialog::setScreenSaverName(const QString &name)
{
    if (name == m_screenSaverName)
        return;

    // 加载翻译
    QString appName = qApp->applicationName();
    qApp->setApplicationName(name);
    qApp->loadTranslator();
    qApp->setApplicationName(appName);

    m_screenSaverName = name;

    QString path = configPath();
    m_backend.reset(new QSettingBackend(path, this));

    path = jsonPath();
    m_settings.reset(DSettings::fromJsonFile(path));
    m_settings->setParent(this);

    m_settings->setBackend(m_backend.get());

    // 虽然该接口是public，接口名称是更新，但是实际上执行的追加
    // 所以每次更换settings时，需要重新创建DSettingsDialog对象
    updateSettings(m_settings.get());
}

QString ConfigSettingsDialog::screenSaverName() const
{
    return m_screenSaverName;
}

QString ConfigSettingsDialog::configPath()
{
    auto paths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    Q_ASSERT(!paths.isEmpty());

    QString path = paths.first();
    path = path
            + "/" + QApplication::organizationName()
            + "/" + "deepin-screensaver"
            + "/" + m_screenSaverName
            + "/" + m_screenSaverName + ".conf";

    return path;
}

QString ConfigSettingsDialog::jsonPath()
{
    auto paths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    Q_ASSERT(!paths.isEmpty());

    QString path = paths.first();
    path = path
            + "/" + QApplication::organizationName()
            + "/" + "deepin-screensaver"
            + "/" + m_screenSaverName
            + "/" + m_screenSaverName + ".json";

    return path;
}

QPair<QWidget *, QWidget *> ConfigSettingsDialog::createSelectWorkPathWidget(QObject *opt)
{
    auto option = qobject_cast<Dtk::Core::DSettingsOption *>(opt);

    ConfigSettingsDialog::m_frame = new DFrame;
    ConfigSettingsDialog::m_pathLabel = new DLabel(ConfigSettingsDialog::m_frame);
    ConfigSettingsDialog::m_btn = new DPushButton(tr("Select"), ConfigSettingsDialog::m_frame);

    ConfigSettingsDialog::m_pathLabel->setElideMode(Qt::ElideMiddle);
    ConfigSettingsDialog::m_pathLabel->setText(option->value().toString());
    ConfigSettingsDialog::m_pathLabel->setToolTip(option->value().toString());

    QHBoxLayout *layout = new QHBoxLayout(ConfigSettingsDialog::m_frame);
    layout->addWidget(ConfigSettingsDialog::m_btn);
    layout->addWidget(ConfigSettingsDialog::m_pathLabel);
    layout->addStretch();

    connect(ConfigSettingsDialog::m_btn, &DPushButton::clicked, option, [=](){
        QString currentPath = ConfigSettingsDialog::m_pathLabel->text();
        if (currentPath.isEmpty())
            currentPath = "/home";

        QString selectPath = DFileDialog::getExistingDirectory(nullptr, tr("Open Directory"), currentPath,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (selectPath.isEmpty())
            return ;

        option->setValue(selectPath);
    });

    connect(option, &DSettingsOption::valueChanged, ConfigSettingsDialog::m_frame, [&](QVariant value) {
        QString currentPath = value.toString();
        ConfigSettingsDialog::m_pathLabel->setText(currentPath);
        ConfigSettingsDialog::m_pathLabel->setToolTip(currentPath);
    });

    return qMakePair(ConfigSettingsDialog::m_frame, nullptr);
}
