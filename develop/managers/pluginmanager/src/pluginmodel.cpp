#include "../include/pluginmodel.h"

#include <QCoreApplication>
#include <QDirIterator>
#include <QDir>
#include <QLibrary>

#include <log.h>
#include <engine.h>
#include <module.h>
#include <system.h>
#include <components/scene.h>

#include <bson.h>

#include <rendergl.h>

#include "assetmanager.h"
#include <projectmanager.h>

enum {
    PLUGIN_NAME,
    PLUGIN_DESCRIPTION,
    PLUGIN_VERSION,
    PLUGIN_PATH
};

#define PATH    "path"
#define DESC    "desc"

#define VERSION "version"

#if defined(Q_OS_MAC)
#define PLUGINS "/../../../plugins"
#else
#define PLUGINS "/plugins"
#endif

typedef Module *(*moduleHandler)  (Engine *engine);

PluginModel *PluginModel::m_pInstance = nullptr;

PluginModel::PluginModel() :
        BaseObjectModel(),
        m_pEngine(nullptr),
        m_pRender(nullptr) {

    m_Suffixes = QStringList() << "*.dll" << "*.dylib" << "*.so";

}

PluginModel::~PluginModel() {
    m_Scenes.clear();

    m_Systems.clear();

    foreach(auto it, m_Extensions) {
        delete it;
    }
    m_Extensions.clear();

    foreach(auto it, m_Libraries) {
        delete it;
    }
    m_Libraries.clear();
}

int PluginModel::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant PluginModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const {
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case PLUGIN_NAME:       return tr("PlugIn Name");
            case PLUGIN_DESCRIPTION:return tr("Description");
            case PLUGIN_VERSION:    return tr("Version");
            case PLUGIN_PATH:       return tr("Full Path");
            default: break;
        }
    }
    return QVariant();
}

QVariant PluginModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }

    QObject *item = static_cast<QObject *>(index.internalPointer());
    switch(role) {
        case Qt::DisplayRole: {
            switch(index.column()) {
                case PLUGIN_NAME:           return item->objectName();
                case PLUGIN_DESCRIPTION:    return item->property(DESC).toString();
                case PLUGIN_PATH:           return item->property(PATH).toString();
                case PLUGIN_VERSION:        return item->property(VERSION).toString();
                default: break;
            }
        } break;
        default: break;
    }

    return QVariant();
}

PluginModel *PluginModel::instance() {
    if(!m_pInstance) {
        m_pInstance = new PluginModel;
    }
    return m_pInstance;
}

void PluginModel::destroy() {
    delete m_pInstance;
    m_pInstance = nullptr;
}

void PluginModel::init(Engine *engine) {
    m_pEngine = engine;

    rescanPath(QString(QCoreApplication::applicationDirPath() + PLUGINS));
}

void PluginModel::rescan() {
    clear();

    QString system(QCoreApplication::applicationDirPath() + PLUGINS);
    for(auto it = m_Libraries.begin(); it != m_Libraries.end(); it++) {
        if(!it.key().contains(system)) {
            PluginsMap::Iterator ext = m_Extensions.find(it.key());
            if(ext != m_Extensions.end()) {
                Module *plugin = ext.value();
                delete plugin;
            }
            if(it.value()->unload()) {
                delete it.value();
            }
            m_Libraries.remove(it.key());
        }
    }

    rescanPath(ProjectManager::instance()->pluginsPath());
}

bool PluginModel::loadPlugin(const QString &path, bool reload) {
    QLibrary *lib = new QLibrary(path);
    if(lib->load()) {
        moduleHandler moduleCreate = reinterpret_cast<moduleHandler>(lib->resolve("moduleCreate"));
        if(moduleCreate) {
            Module *plugin = moduleCreate(m_pEngine);
            if(plugin) {
                m_Libraries[path] = lib;

                uint8_t types = plugin->types();
                if(types & Module::SYSTEM) {
                    registerExtensionPlugin(path, plugin);
                    registerSystem(plugin);
                }
                if(types & Module::EXTENSION) {
                    registerExtensionPlugin(path, plugin);
                }
                if(types & Module::CONVERTER) {
                     AssetManager::instance()->registerConverter(plugin->converter());
                }
                if(!reload) {
                    ComponentMap result;
                    serializeComponents(plugin->components(), result);
                    deserializeComponents(result);
                }

                int start = rowCount();
                beginInsertRows(QModelIndex(), start, start);
                    QFileInfo file(path);

                    QObject *item = new QObject(m_rootItem);
                    item->setObjectName(file.fileName());
                    item->setProperty(PATH, file.filePath());
                    item->setProperty(VERSION, plugin->version());
                    item->setProperty(DESC, plugin->description());
                endInsertRows();
                return true;
            } else {
                Log(Log::ERR) << "[ PluginModel::loadPlugin ] Can't create plugin:" << qPrintable(lib->fileName());
            }
        } else {
            Log(Log::ERR) << "[ PluginModel::loadPlugin ] Bad plugin:" << qPrintable(lib->fileName());
        }
    } else {
        Log(Log::ERR) << "[ PluginModel::loadPlugin ] Can't load plugin:" << qPrintable(lib->fileName());
    }
    delete lib;
    return false;
}

void PluginModel::reloadPlugin(const QString &path) {
    QFileInfo info(path);

    QFileInfo dest = ProjectManager::instance()->pluginsPath() + QDir::separator() + info.fileName();
    QFileInfo temp = dest.absoluteFilePath() + ".tmp";

    // Rename old version of plugin
    if(dest.exists()) {
        QFile::remove(temp.absoluteFilePath());
        QFile::rename(dest.absoluteFilePath(), temp.absoluteFilePath());
    }

    PluginsMap::Iterator ext = m_Extensions.find(dest.absoluteFilePath());
    LibrariesMap::Iterator lib = m_Libraries.find(dest.absoluteFilePath());

    if(ext != m_Extensions.end() && lib != m_Libraries.end()) { // Reload plugin
        Module *plugin = ext.value();
        ComponentMap result;
        serializeComponents(plugin->components(), result);
        // Unload plugin
        delete plugin;

        QObject *child = m_rootItem->findChild<QObject *>(info.fileName());
        if(child) {
            child->deleteLater();
        }

        if(lib.value()->unload()) {
            // Copy new plugin
            if(QFile::copy(path, dest.absoluteFilePath()) && loadPlugin(dest.absoluteFilePath(), true)) {
                deserializeComponents(result);
                // Remove old plugin
                if(QFile::remove(temp.absoluteFilePath())) {
                    Log(Log::INF) << "Plugin:" << qPrintable(path) << "reloaded";
                    return;
                }
            }
            delete lib.value();
            m_Extensions.remove(ext.key());
        } else {
            Log(Log::ERR) << "Plugin unload:" << qPrintable(path) << "failed";
        }
    } else { // Just copy and load plugin
        if(QFile::copy(path, dest.absoluteFilePath()) && loadPlugin(dest.absoluteFilePath())) {
            Log(Log::INF) << "Plugin:" << qPrintable(path) << "simply loaded";
            return;
        }
    }
    // Rename it back
    if(QFile::remove(dest.absoluteFilePath()) && QFile::rename(temp.absoluteFilePath(), dest.absoluteFilePath())) {
        if(loadPlugin(dest.absoluteFilePath())) {
            Log(Log::INF) << "Old version of plugin:" << qPrintable(path) << "is loaded";
        } else {
            Log(Log::ERR) << "Load of old version of plugin:" << qPrintable(path) << "is failed";
        }
    }
}

void PluginModel::clear() {
    delete m_rootItem;
    m_rootItem = createRoot();

    beginResetModel();
    endResetModel();
}

void PluginModel::rescanPath(const QString &path) {
    QDirIterator it(path, m_Suffixes, QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        loadPlugin(it.next());
    }
}

void PluginModel::registerSystem(Module *plugin) {
    System *system = plugin->system();
    m_Systems[QString::fromStdString(system->name())] = system;
    if(QString(system->name()) == "RenderGL") {
        m_pRender = system;
    }
}

void PluginModel::initSystems() {
    foreach(auto it, m_Systems) {
        if(it) {
            it->init();
        }
    }
}

void PluginModel::updateSystems(Scene *scene) {
    foreach(auto it, m_Systems) {
        if(it) {
            it->setActiveScene(scene);
            it->processEvents();
        }
    }
}

void PluginModel::updateRender(Scene *scene) {
    m_pEngine->resourceSystem()->processEvents();

    if(m_pRender) {
        m_pRender->setActiveScene(scene);
        m_pRender->processEvents();
    }
}

void PluginModel::registerExtensionPlugin(const QString &path, Module *plugin) {
    m_Extensions[path] = plugin;
    emit updated();
}

void PluginModel::addScene(Scene *scene) {
    m_Scenes.push_back(scene);
}

typedef list<const Object *> ObjectArray;
void enumComponents(const Object *object, const string &type, ObjectArray &list) {
    for(const auto &it : object->getChildren()) {
        if(it->typeName() == type) {
            list.push_back(it);
        } else {
            enumComponents(it, type, list);
        }
    }
}

void PluginModel::serializeComponents(const StringList &list, ComponentMap &map) {
    for(auto type : list) {
        foreach(Scene *scene, m_Scenes) {
            ObjectArray array;

            enumComponents(scene, type, array);

            for(auto it : array) {
                Variant v = Engine::toVariant(it);
                map[it->parent()] = Bson::save(v);
                delete it;
            }
        }
    }
}

void PluginModel::deserializeComponents(const ComponentMap &map) {
    auto it = map.constBegin();
    while(it != map.constEnd()) {
        Variant v = Bson::load(it.value());
        Object *object = Engine::toObject(v);
        if(object) {
            object->setParent(it.key());
        }
        ++it;
    }
    emit pluginReloaded();
}
