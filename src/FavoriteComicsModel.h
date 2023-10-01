/**
 * Copyright (c) 2015 Damien Tardy-Panis
 *
 * This file is subject to the terms and conditions defined in
 * file `LICENSE.txt`, which is part of this source code package.
 **/

#ifndef FAVORITECOMICSMODEL_H
#define FAVORITECOMICSMODEL_H

#include <QSharedPointer>

#include "ComicsModel.h"

class ComicsInfoUpdater;

class FavoriteComicsModel : public ComicsModel
{
    Q_OBJECT

    QSharedPointer<ComicsInfoUpdater> m_comicsInfoUpdater;
public:
    explicit FavoriteComicsModel(QObject *parent = 0);
    Q_INVOKABLE void removeFavorite(int row);

signals:
    void favoritesChanged();
public slots:
    Q_INVOKABLE virtual void fetchAll() override;
private slots:
    void handleGotUpdateInfo(const QJsonDocument& updateInfo);

protected:
    virtual QStringList idLoadList() override;
private:
    void updateComics(const QJsonDocument& updateInfo);
};

#endif // FAVORITECOMICSMODEL_H
