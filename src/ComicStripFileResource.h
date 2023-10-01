/**
 * Copyright (c) 2015 Damien Tardy-Panis <damien@tardypad.me>
 *
 * This file is subject to the terms and conditions defined in
 * file `LICENSE.txt`, which is part of this source code package.
 **/

#ifndef COMICSTRIPFILERESOURCE_H
#define COMICSTRIPFILERESOURCE_H

#include <QObject>

class Comic;

class ComicStripFileResource : public QObject
{
    Q_OBJECT

    static const QString _comicsDirName;

public:
    static ComicStripFileResource* instance();
    QString path(QString id);
    bool isDownloaded(QString id);
    bool save(QString id, QByteArray data);
    bool saveToGallery(QString id);

private:
    explicit ComicStripFileResource(QObject *parent = 0);
    QString dirPath();
    QString picturesDirPath();
    void checkCreateStructure();

private:
    static ComicStripFileResource* m_instance;
    QString m_dirPath;
    QString m_picturesDirPath;
};

#endif // COMICSTRIPFILERESOURCE_H
