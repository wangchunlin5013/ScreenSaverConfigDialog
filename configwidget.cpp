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
#include "configwidget.h"
#include "screensaverlistview.h"
#include "configsettingsdialog.h"

#include <QHBoxLayout>
#include <QStandardPaths>
#include <QModelIndex>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>

DWIDGET_USE_NAMESPACE

ConfigWidget::ConfigWidget(QWidget *parent)
    : DWidget(parent)
{
    m_listView.reset(new ScreenSaverListView);
    m_settingDialog.reset(new ConfigSettingsDialog);

    m_hLayout.reset(new QHBoxLayout(this));
    m_hLayout->addWidget(m_listView.get());
    m_hLayout->addWidget(m_settingDialog.get());

    setLayout(m_hLayout.get());

    QString path = programPath();

    QDir workDir(path);

    // 添加数据
    QStringList screenSaverList = workDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // 确保界面配置文件存在且可读
    for (auto screen : screenSaverList) {
        QString jsonFilePath = jsonPath();
        jsonFilePath += screen + "/" + screen + ".json";
        QFileInfo jsonFile(jsonFilePath);
        if (!jsonFile.exists() || !jsonFile.isReadable()) {
            qDebug() << "notes:" << screen << " json is " << jsonFile.exists() << jsonFile.isReadable();
            screenSaverList.removeOne(screen);
        }
    }
    m_listView->setDatas(screenSaverList);

    connect(m_listView.get(), &ScreenSaverListView::sigSelectChanged, this, &ConfigWidget::onSelectScreenSaverChanged);

    // 初始化完成后选中第一个
    const QModelIndex &index = m_listView->model()->index(0, 0);
    if (Q_LIKELY(index.isValid())) {
        m_listView->setCurrentIndex(index);
    }
}

ConfigWidget::~ConfigWidget()
{

}

void ConfigWidget::onSelectScreenSaverChanged(const QString &name)
{
    m_hLayout->removeWidget(m_settingDialog.get());

    m_settingDialog.reset(new ConfigSettingsDialog);

    m_hLayout->addWidget(m_settingDialog.get());

    m_settingDialog->setScreenSaverName(name);
}

QString ConfigWidget::programPath()
{
    QString path("/usr/lib/deepin-screensaver/modules");

    return path;
}

QString ConfigWidget::jsonPath()
{
    QString path("/etc/deepin-screensaver/");

    return path;
}


