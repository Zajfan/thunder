#include "fontconverter.h"

#include <QFile>

#include <bson.h>

#include "resources/font.h"

#define DATA    "Data"

AssetConverter::ReturnCode FontConverter::convertFile(AssetConverterSettings *settings) {
    QFile src(settings->source());
    if(src.open(QIODevice::ReadOnly)) {
        Font font;

        QByteArray data = src.readAll();
        src.close();

        ByteArray m_Data;
        m_Data.resize(data.size());
        memcpy(&m_Data[0], data.data(), data.size());

        VariantMap map;
        map[DATA] = m_Data;
        font.loadUserData(map);

        QFile file(settings->absoluteDestination());
        if(file.open(QIODevice::WriteOnly)) {
            ByteArray data = Bson::save( Engine::toVariant(&font) );
            file.write((const char *)&data[0], data.size());
            file.close();
            return Success;
        }
    }
    return InternalError;
}

AssetConverterSettings *FontConverter::createSettings() const {
    AssetConverterSettings *result = AssetConverter::createSettings();
    result->setType(MetaType::type<Font *>());
    return result;
}
