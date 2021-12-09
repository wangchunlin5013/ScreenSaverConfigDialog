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
#ifndef SCREENSAVERLISTVIEW_H
#define SCREENSAVERLISTVIEW_H

#include <QScopedPointer>
#include <QItemSelectionModel>
#include <DListView>

class QStringListModel;

class ScreenSaverListView : public Dtk::Widget::DListView
{
    Q_OBJECT
public:
    explicit ScreenSaverListView(QWidget *parent = nullptr);
    ~ScreenSaverListView();

    void setDatas(const QStringList &datas);

signals:
    void sigSelectChanged(const QString &screenSaverName);

private slots:
    void onSelectChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QScopedPointer<QStringListModel> m_stringListMode;
};

#endif // SCREENSAVERLISTVIEW_H
