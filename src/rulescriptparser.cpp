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

#include "rulescriptparser.h"
#include "main.h"
#include <QSettings>

RuleScriptParser::RuleScriptParser()
{
}

bool RuleScriptParser::writeHolderToSettings(RuleHolder &holder, QSettings &settings, QString section)
{
    if(!holder.isValid())return false;
    settings.beginGroup(section);
    settings.setValue("ComparationText",holder.comparationText);
    settings.setValue("ThanAmount",holder.thanAmount);
    settings.setValue("ThanAmountFeeIndex",holder.thanAmountFeeIndex);
    settings.setValue("ThanAmountPercentChecked",holder.thanAmountPercentChecked);
    settings.setValue("ThanPrice",holder.thanPrice);
    settings.setValue("ThanPriceFeeIndex",holder.thanPriceFeeIndex);
    settings.setValue("ThanPricePercentChecked",holder.thanPricePercentChecked);
    settings.setValue("ThanPricePlusMinusText",holder.thanPricePlusMinusText);
    settings.setValue("ThanPriceTypeCode",holder.thanPriceTypeCode);
    settings.setValue("ThanText",holder.thanText);
    settings.setValue("ThanTypeIndex",holder.thanTypeIndex);
    settings.setValue("TradeSymbolCode",holder.tradeSymbolCode);
    settings.setValue("ValueASymbolCode",holder.valueASymbolCode);
    settings.setValue("ValueBSymbolCode",holder.valueBSymbolCode);
    settings.setValue("VariableACode",holder.variableACode);
    settings.setValue("VariableBCode",holder.variableBCode);
    settings.setValue("VariableBExact",holder.variableBExact);
    settings.setValue("VariableBFeeIndex",holder.variableBFeeIndex);
    settings.setValue("VariableBModeIndex",holder.variableBModeIndex);
    settings.setValue("VariableBPercentChecked",holder.variableBPercentChecked);
    settings.setValue("VariableBplusMinus",holder.variableBplusMinus);
    settings.setValue("VariableBSymbolCode",holder.variableBSymbolCode);
    settings.setValue("Description",holder.description);
    settings.endGroup();
    settings.sync();
    return true;
}

bool RuleScriptParser::writeHolderToFile(RuleHolder &holder, QString &file, QString section)
{
    QSettings settings(file,QSettings::IniFormat);
    return writeHolderToSettings(holder,settings,section);
}

RuleHolder RuleScriptParser::readHolderFromSettings(QSettings &settings, QString section)
{
    RuleHolder holder;
    settings.beginGroup(section);
    holder.comparationText=settings.value("ComparationText","").toString();
    holder.thanAmount=settings.value("ThanAmount",0.0).toDouble();
    holder.thanAmountFeeIndex=settings.value("ThanAmountFeeIndex",-1).toInt();
    holder.thanAmountPercentChecked=settings.value("ThanAmountPercentChecked",false).toBool();
    holder.thanPrice=settings.value("ThanPrice",0.0).toDouble();
    holder.thanPriceFeeIndex=settings.value("ThanPriceFeeIndex",-1).toInt();
    holder.thanPricePercentChecked=settings.value("ThanPricePercentChecked",false).toBool();
    holder.thanPricePlusMinusText=settings.value("ThanPricePlusMinusText","").toString();
    holder.thanPriceTypeCode=settings.value("ThanPriceTypeCode","").toString();
    holder.thanText=settings.value("ThanText","").toString();
    holder.thanTypeIndex=settings.value("ThanTypeIndex",-1).toInt();
    holder.tradeSymbolCode=settings.value("TradeSymbolCode","").toString();
    holder.valueASymbolCode=settings.value("ValueASymbolCode","").toString();
    holder.valueBSymbolCode=settings.value("ValueBSymbolCode","").toString();
    holder.variableACode=settings.value("VariableACode","").toString();
    holder.variableBCode=settings.value("VariableBCode","").toString();
    holder.variableBExact=settings.value("VariableBExact",0.0).toDouble();
    holder.variableBFeeIndex=settings.value("VariableBFeeIndex",-1).toInt();
    holder.variableBModeIndex=settings.value("VariableBModeIndex",-1).toInt();
    holder.variableBPercentChecked=settings.value("VariableBPercentChecked",false).toBool();
    holder.variableBplusMinus=settings.value("VariableBplusMinus","").toString();
    holder.variableBSymbolCode=settings.value("VariableBSymbolCode","").toString();
    holder.description=settings.value("Description","").toString();
    settings.endGroup();
    return holder;
}

RuleHolder RuleScriptParser::readHolderFromFile(QString &file, QString section)
{
    QSettings settings(file,QSettings::IniFormat);
    return readHolderFromSettings(settings,section);
}

QString RuleScriptParser::holderToScript(RuleHolder &holder, bool testMode)
{
    QString script;

    QString executingCode;
    if(!testMode)
    {
        if(holder.thanTypeIndex<4)
        {
            qreal amount=holder.thanAmount;
            if(holder.thanAmountPercentChecked)
            {
                amount/=100.0;
                if(holder.thanTypeIndex==0)
                    executingCode+=" var amount = trader.get(\"Balance\",\""+baseValues.currentPair.currAStr+"\");\n";
                else
                    executingCode+=" var amount = trader.get(\"Balance\",\""+baseValues.currentPair.currBStr+"\");\n";
                if(amount!=0.0&&amount!=1.0)
                    executingCode+=" amount *= "+mainWindow.numFromDouble(amount)+";\n";
            }
            else executingCode+=" var amount = "+mainWindow.numFromDouble(amount)+";\n";
            executingCode+=" trader.groupDone();\n";

            if(amount!=0.0)
            {
                if(holder.thanAmountFeeIndex==1)executingCode+=" amount *= (1.0 + trader.get(\"Fee\") / 100.0);\n";
                if(holder.thanAmountFeeIndex==2)executingCode+=" amount *= (1.0 - trader.get(\"Fee\") / 100.0);\n";
            }
            if(!executingCode.isEmpty())executingCode+="\n";

            if(holder.thanPriceTypeCode=="EXACT")executingCode+=" var price = "+mainWindow.numFromDouble(holder.thanPrice)+";\n";
            else
            {
                executingCode+=" var price = trader.get(\""+holder.tradeSymbolCode+"\" , \""+holder.thanPriceTypeCode+"\");\n";

                if(holder.thanPricePercentChecked)executingCode+=" price "+holder.thanPricePlusMinusText+"= price * "+mainWindow.numFromDouble(holder.thanPrice/100.0)+";\n";
                else if(holder.thanPrice!=0.0)executingCode+=" price "+holder.thanPricePlusMinusText+"= "+mainWindow.numFromDouble(holder.thanPrice)+";\n";

                if(holder.thanPriceFeeIndex==1)executingCode+=" price *= ( 1.0 + trader.get(\"Fee\") / 100.0 );\n";
                if(holder.thanPriceFeeIndex==2)executingCode+=" price *= ( 1.0 - trader.get(\"Fee\") / 100.0 );\n";
            }
            if(!executingCode.isEmpty())executingCode+="\n";
            switch(holder.thanTypeIndex)
            {
            case 0: //Sell
                executingCode+=" trade.sell(\""+holder.tradeSymbolCode+"\" , amount , price)";
                break;
            case 1: //Buy
                if(holder.thanAmountPercentChecked)
                    executingCode+=" trade.buy(\""+holder.tradeSymbolCode+"\" , amount / price , price)";
                else
                    executingCode+=" trade.buy(\""+holder.tradeSymbolCode+"\" , amount , price)";
                break;
            case 2: //Receive
                executingCode+=" trade.sell(\""+holder.tradeSymbolCode+"\" , amount / price , price)";
                break;
            case 3: //Spend
                executingCode+=" trade.buy(\""+holder.tradeSymbolCode+"\" , amount / price , price)";
                break;
            default: break;
            }
            if(!executingCode.isEmpty())executingCode+=";\n";
        }
        else
        {
            executingCode+=" trader.groupDone();\n";
            switch(holder.thanTypeIndex)
            {
            case 4: //Cancel all Orders
                executingCode+=" trader.cancelOrders();\n";
                break;
            case 5: //Cancel Asks
                executingCode+=" trader.cancelAsks();\n";
                break;
            case 6: //Cancel Bids
                executingCode+=" trader.cancelBids();\n";
                break;
            case 7://Start group
                executingCode+=" trader.groupStart(\""+holder.thanText+"\");\n";
                break;
            case 8://Stop group
                executingCode+=" trader.groupStop(\""+holder.thanText+"\");\n";
                break;
            case 9://Beep
                executingCode+=" trader.beep();\n";
                break;
            case 10://Play Sound
                executingCode+=" trader.playWav(\""+holder.thanText+"\");\n";
                break;
            case 11://Start app
                executingCode+=" trader.startApp(\""+holder.thanText+"\");\n";
                break;
            }
        }
    }
    else executingCode=" trader.test(1);\n trader.groupStop();\n";

    if(holder.variableACode=="IMMEDIATELY")
    {
        if(executingCode.startsWith(" "))executingCode.remove(0,1);
        script=executingCode.replace("\n ","\n");
    }
    else
    {
    QString indicatorBValue;
    QString indicatorB;

    if(holder.variableBCode=="EXACT")indicatorB=mainWindow.numFromDouble(holder.variableBExact)+";\n";
    else
    {
        indicatorBValue="trader.get(\""+holder.variableBSymbolCode+"\" , \""+holder.variableBCode+"\")";
        indicatorB=indicatorBValue+";\n";
        if(holder.variableBPercentChecked)
            indicatorB+=" baseVariable "+holder.variableBplusMinus+"= baseVariable*"+mainWindow.numFromDouble(holder.variableBExact/100.0)+";\n";
        else
            if(holder.variableBExact!=0.0)indicatorB+=" baseVariable "+holder.variableBplusMinus+"= "+mainWindow.numFromDouble(holder.variableBExact)+";\n";

        if(holder.variableBFeeIndex>0)
        {
            QString sign=(holder.variableBFeeIndex==1?"+":"-");
            indicatorB+=" baseVariable "+sign+"= baseVariable*trader.get(\""+holder.valueBSymbolCode+"\" , \"Fee\");\n";
        }
    }

    QString realtime;
    if(holder.variableBModeIndex==0)realtime=" calcBaseVariable();\n";
    else
    if(holder.variableBModeIndex==2)
    {
        bool haveLessThan=holder.comparationText.contains("<");
        bool haveMoreThan=holder.comparationText.contains(">");
        if(haveLessThan)realtime=" if(value > "+indicatorBValue+")calcBaseVariable();\n";
        if(haveMoreThan)realtime=" if(value < "+indicatorBValue+")calcBaseVariable();\n";
    }

    script="var baseVariable = calcBaseVariable();\n"
    "function calcBaseVariable()\n"
    "{\n"
    " baseVariable = "+indicatorB+
    " return baseVariable;\n"
    "}\n\n"
    "trader.on(\""+holder.variableACode+"\").changed()\n"
    "{\n"
    " if(symbol != \""+holder.valueASymbolCode+"\")return;\n"
    +realtime+
    " if(value "+holder.comparationText+" baseVariable)\n"
    " {\n"+executingCode+
    " }\n";
    if(testMode)script+="else\n{ trader.test(2); trader.stopGroup(); }\n";
    script+="}\n";
    }
    return script;
}
