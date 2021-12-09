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
#include "screensaverlistview.h"

#include <QStringListModel>

DWIDGET_USE_NAMESPACE

ScreenSaverListView::ScreenSaverListView(QWidget *parent)
    : DListView(parent)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_stringListMode.reset(new QStringListModel);

    setModel(m_stringListMode.get());

    setSelectionMode(QAbstractItemView::SingleSelection);

    QItemSelectionModel *selectionModel = this->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &ScreenSaverListView::onSelectChanged);
}

ScreenSaverListView::~ScreenSaverListView()
{

}

void ScreenSaverListView::setDatas(const QStringList &datas)
{
    m_stringListMode->setStringList(datas);
}

void ScreenSaverListView::onSelectChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    const QModelIndexList &list = selected.indexes();
    if (list.count() != 1) {
        qDebug() << "error:only one can be selected,but now:" << list.count();
        return;
    }

    QString name = m_stringListMode->data(list.first()).toString();

    emit sigSelectChanged(name);
}
