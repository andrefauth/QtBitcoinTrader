//  This file is part of Qt Bitcion Trader
//      https://github.com/JulyIGHOR/QtBitcoinTrader
//  Copyright (C) 2013-2014 July IGHOR <julyighor@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  In addition, as a special exception, the copyright holders give
//  permission to link the code of portions of this program with the
//  OpenSSL library under certain conditions as described in each
//  individual source file, and distribute linked combinations including
//  the two.
//
//  You must obey the GNU General Public License in all respects for all
//  of the code used other than OpenSSL. If you modify file(s) with this
//  exception, you may extend this exception to your version of the
//  file(s), but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version. If you delete
//  this exception statement from all source files in the program, then
//  also delete it here.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DEPTHMODEL_H
#define DEPTHMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include "depthitem.h"

class DepthModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	void reloadVisibleItems();
	void fixTitleWidths();
	int itemsCount(){return volumeList.count();}
	void calculateSize();
	void clear();
	void setHorizontalHeaderLabels(QStringList list);
	void setAsk(bool on){isAsk=on;};
	DepthModel(bool isAskData=true);
	~DepthModel();
	qreal rowPrice(int row);
	qreal rowVolume(int row);
	qreal rowSize(int row);

	QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void depthUpdateOrders(QList<DepthItem> *items);
	void depthFirstOrder(qreal price, qreal volume);

private slots:
	void delayedReloadVisibleItems();
private:
    void depthUpdateOrder(DepthItem item);
	bool originalIsAsk;
	bool somethingChanged;
	qreal groupedPrice;
	qreal groupedVolume;

	int widthPrice;
	int widthVolume;
	int widthSize;

	int widthPriceTitle;
	int widthVolumeTitle;
	int widthSizeTitle;

	int columnsCount;
	QStringList headerLabels;
	bool isAsk;
	QList<qreal> volumeList;
	QList<qreal> sizeList;
	QList<qreal> priceList;
	QStringList volumeListStr;
	QStringList sizeListStr;
	QStringList priceListStr;
	QList<int> directionList;

protected:
};

#endif // DEPTHMODEL_H