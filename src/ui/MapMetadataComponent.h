#pragma once
#include <qobject.h>
#include "PK2MapBaseMetadata.h"

class MapMetadataComponent : public QObject {
	Q_OBJECT

public slots:
	// Use the metadata pointer to set the MapMetadataComponents data
	virtual void mapMetadataChanged(const PK2::MapBaseMetadata* metadata) = 0;

signals:
	void mapChanged();

};