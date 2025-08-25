/******************************************************************************
 * Copyright (c) 2025 OpenVPI                                                 *
 *                                                                            *
 * This file is part of SVSCraft                                              *
 *                                                                            *
 * SVSCraft is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * SVSCraft is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public        *
 * License for more details.                                                  *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with SVSCraft. If not, see <https://www.gnu.org/licenses/>.          *
 ******************************************************************************/

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QColor>
#include <QSurfaceFormat>
#include <QQuickStyle>
#include <QStandardItemModel>
#include <qqmlcontext.h>

#include <SVSCraftQuick/Theme.h>
#include <SVSCraftGui/Eyedropper.h>
#include <SVSCraftCore/SVSCraftNamespace.h>


using namespace SVS;

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    auto sf = QSurfaceFormat::defaultFormat();
    sf.setSamples(8);
    QSurfaceFormat::setDefaultFormat(sf);

    QQuickStyle::setStyle("SVSCraft.UIComponents");
    QQuickStyle::setFallbackStyle("Basic");

    // Create command palette model
    auto commandPaletteModel = new QStandardItemModel(&a);
    
    // Add test items
    struct CommandItem {
        QString title;
        QString subtitle;
        QString description;
        QString keySequence;
        QString tag;
    };
    
    QList<CommandItem> commands = {
        {"Test 1", "test 1", "Description of Test 1", "Ctrl+A", "recently used"},
        {"Test 2", "test 2", "Description of Test 2", "Ctrl+B"},
        {"Test 3", "test 3", "Description of Test 3", ""},
        {"Test 4", "test 4", "Description of Test 4", ""},
        {"Test 5", "test 5", "Description of Test 5", ""},
        {"Test 6", "test 6", "Description of Test 6", ""},
        {"Test 7", "test 7", "Description of Test 7", ""},
        {"Test 8", "", "Description of Test 8", ""},
    };
    
    for (const auto &cmd : commands) {
        auto item = new QStandardItem;
        item->setData(cmd.title, SVSCraft::CP_TitleRole);
        item->setData(cmd.subtitle, SVSCraft::CP_SubtitleRole);
        item->setData(cmd.description, SVSCraft::CP_DescriptionRole);
        item->setData(cmd.keySequence, SVSCraft::CP_KeySequenceRole);
        item->setData(cmd.tag, SVSCraft::CP_TagRole);
        item->setData(cmd.title + " " + cmd.subtitle, SVSCraft::CP_KeywordRole);
        commandPaletteModel->appendRow(item);
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("commandPaletteModel", commandPaletteModel);
    engine.load(":/qt/qml/SVSCraft/Test/UIComponents/main.qml");

    return a.exec();
}
