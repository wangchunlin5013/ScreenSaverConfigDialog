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

    QString path = workPath();

    QDir workDir(path);

    // 添加数据
    QStringList screenSaverList = workDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

    // 确保存在界面配置文件
    for (auto screen : screenSaverList) {
        QString filePath(path + screen + "/" + screen + ".json");
        QFile jsonFile(filePath);
        if (!jsonFile.exists()) {
            qDebug() << "Warning:" << screen << " not found json";
            qDebug() << screen << jsonFile.exists() << jsonFile.isReadable();
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

QString ConfigWidget::workPath()
{
    auto paths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    Q_ASSERT(!paths.isEmpty());

    QString path = paths.first();
    path = path
            + "/" + QApplication::organizationName()
            + "/" + "deepin-screensaver/";

    return path;
}


