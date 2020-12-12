#include "resources/angelscript.h"

#define DATA    "Data"

void AngelScript::loadUserData(const VariantMap &data) {
    auto it = data.find(DATA);
    if(it != data.end()) {
        m_Array = (*it).second.toByteArray();
    }
}

VariantMap AngelScript::saveUserData() const {
    VariantMap result;

    result[DATA]  = m_Array;

    return result;
}
