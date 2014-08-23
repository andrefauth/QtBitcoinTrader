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

#ifndef EXCHANGE_BITFINEX_H
#define EXCHANGE_BITFINEX_H

#include "exchange.h"

class Exchange_Bitfinex : public Exchange
{
	Q_OBJECT

public:
	Exchange_Bitfinex(QByteArray pRestSign, QByteArray pRestKey);
	~Exchange_Bitfinex();

private:
	QByteArray historyLastTimestamp;

	bool isFirstAccInfo;
	bool isFirstTicker;
	bool isReplayPending(int);
	bool lastInfoReceived;

	int apiDownCounter;
	int secondPart;

	JulyHttp *julyHttp;

	QByteArray lastTradesDateCache;

	quint64 lastTradesDate;
	quint32 tickerLastDate;

	QList<DepthItem> *depthAsks;
	QList<DepthItem> *depthBids;

	QMap<qreal,qreal> lastDepthAsksMap;
	QMap<qreal,qreal> lastDepthBidsMap;

	QString apiLogin;

	QTime authRequestTime;

	quint32 privateNonce;

	void clearVariables();
    void depthSubmitOrder(QString,QMap<qreal,qreal> *,qreal,qreal,bool);
    void depthUpdateOrder(QString, qreal,qreal,bool);
	void sendToApi(int reqType, QByteArray method, bool auth=false, bool sendNow=true, QByteArray commands=0);
private slots:
	void secondSlot();
public slots:
	void dataReceivedAuth(QByteArray,int);
	void reloadDepth();
	void clearValues();
	void getHistory(bool);
    void buy(QString, qreal, qreal);
    void sell(QString, qreal, qreal);
    void cancelOrder(QString, QByteArray);
};

#endif // EXCHANGE_BITFINEX_H