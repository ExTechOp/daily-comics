/**
 * Copyright (c) 2016 Damien Tardy-Panis <damien@tardypad.me>
 *
 * This file is subject to the terms and conditions defined in
 * file `LICENSE.txt`, which is part of this source code package.
 **/

#include "ComicPluginResource.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QQmlEngine>
#include <QJSValue>
#include <QJSValueList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QColor>
#include <QLocale>

const QString ComicPluginResource::_pluginsFolderPath = QString(PLUGINS_FOLDER_PATH);
const QString ComicPluginResource::_infoFileName = "info.json";
const QString ComicPluginResource::_coverFilename = "cover.jpg";
const QString ComicPluginResource::_exampleFilename = "example.jpg";
const QString ComicPluginResource::_extractScriptFilename = "extract.js";

ComicPluginResource* ComicPluginResource::m_instance = NULL;

ComicPluginResource::ComicPluginResource(QObject *parent) :
	QObject(parent),
	_jsEngine(new QQmlEngine(this))
{
}

ComicPluginResource* ComicPluginResource::instance()
{
	if (!m_instance) {
		m_instance = new ComicPluginResource();
	}

	return m_instance;
}

void ComicPluginResource::load(Comic *comic)
{
	loadInfo(comic);
	loadCoverPath(comic);
	loadExamplePath(comic);
}

void ComicPluginResource::loadInfo(Comic *comic)
{
	QString infoFilePath = path(comic->id(), _infoFileName);
	QFile infoFile(infoFilePath);

	if (!infoFile.open(QIODevice::ReadOnly))
		return;

	QByteArray json = infoFile.readAll();
	infoFile.close();

	ComicInfo info = parseInfo(json);
	comic->setInfo(info);
}

void ComicPluginResource::loadCoverPath(Comic *comic)
{
	QString coverFilePath = path(comic->id(), _coverFilename);

	comic->setCoverPath(coverFilePath);
}

void ComicPluginResource::loadExamplePath(Comic *comic)
{
	QString exampleFilePath = path(comic->id(), _exampleFilename);

	comic->setExamplePath(exampleFilePath);
}

QString ComicPluginResource::path(QString id, QString filename)
{
	return QDir(_pluginsFolderPath).path()
			.append(QDir::separator()).append(id)
			.append(QDir::separator()).append(filename);
}

ComicInfo ComicPluginResource::parseInfo(QByteArray infoData)
{
	ComicInfo info;

	QJsonDocument jsonDoc = QJsonDocument::fromJson(infoData);
	QJsonObject infoJson = jsonDoc.object();

	info.name           = infoJson.value("name").toString();
	info.homepage       = infoJson.value("homepage").toString();
	info.stripSourceUrl = infoJson.value("stripSource").toString();
	info.color          = QColor('#' + infoJson.value("color").toString());
	info.language       = QLocale(infoJson.value("language").toString()).language();

	foreach (const QJsonValue & author, infoJson.value("authors").toArray())
		info.authors << author.toString();

	return info;
}

QUrl ComicPluginResource::extractStripImageUrl(Comic *comic, QByteArray data)
{
	QByteArray script;
	if (comic->extractScript().isEmpty())
	{
		QString scriptFilePath = path(comic->id(), _extractScriptFilename);
		QFile scriptFile(scriptFilePath);

		if (!scriptFile.open(QIODevice::ReadOnly))
		{
			return QUrl();
		}

		script = scriptFile.readAll();
		scriptFile.close();
	}
	else
	{
		script = comic->extractScript().toUtf8();
	}

	QJSValue function = _jsEngine->evaluate(script);
	QJSValue result = function.call(QJSValueList() << QString(data));

	QUrl src = result.toVariant().toUrl();

	return src;
}
