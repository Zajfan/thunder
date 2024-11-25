#ifndef SCENECOMPOSER_H
#define SCENECOMPOSER_H

#include <editor/asseteditor.h>

#include <QMenu>

class NextObject;
class ObjectController;
class WorldObserver;

class QLineEdit;
class QToolButton;

namespace Ui {
    class SceneComposer;
}

class SceneComposer : public AssetEditor {
    Q_OBJECT

public:
    explicit SceneComposer(QWidget *parent = nullptr);
    ~SceneComposer();

    VariantMap saveState() override;
    void restoreState(const VariantMap &data) override;

    void backupScenes();
    void restoreBackupScenes();

    void takeScreenshot();

    World *currentWorld() const;
    void worldUpdated(World *graph);

    QMenu *objectContextMenu(Object *object) override;

    QWidget *propertiesWidget() const override;

    QList<QWidget *> createActionWidgets(Object *object, QWidget *parent) const override;

private slots:
    void onActivated() override;

    void onNewAsset() override;
    void onSave() override;
    void onSaveAs() override;

    void onUpdated() override;

    void onChangeSnap();

    void onObjectCreate(QString type) override;
    void onObjectsSelected(QList<Object *> objects, bool force) override;
    void onObjectsDeleted(QList<Object *> objects) override;

    void onObjectsChanged(const QList<Object *> &objects, QString property, const Variant &value) override;

    void onDrop(QDropEvent *event) override;
    void onDragEnter(QDragEnterEvent *event) override;
    void onDragMove(QDragMoveEvent *event) override;
    void onDragLeave(QDragLeaveEvent *event) override;

    void onDropMap(QString name, bool additive);

    void onLocal(bool flag);

    void onSetActiveScene();

    void onRepickSelected();

    void onCreateActor();
    void onActorDelete();
    void onActorDuplicate();
    void onPrefabIsolate();
    void onPrefabUnpack();
    void onPrefabUnpackCompletely();

    void onSaveIsolated();

    void onSaveAll();

    void onRemoveScene();
    void onDiscardChanges();

    void onDeleteComponent();

private:
    void loadAsset(AssetConverterSettings *settings) override;
    void saveAsset(const QString &path = QString()) override;

    bool isModified() const override;
    void setModified(bool flag) override;

    QStringList suffixes() const override;

    QStringList componentGroups() const override;

    bool loadScene(QString path, bool additive);
    void saveScene(QString path, Scene *scene);
    void saveSceneAs(Scene *scene);

    void enterToIsolation(AssetConverterSettings *settings);
    void quitFromIsolation();

    QAction *createAction(const QString &name, const char *member, bool single, const QKeySequence &shortcut = 0);

private:
    Ui::SceneComposer *ui;

    QMenu m_actorMenu;
    QMenu m_sceneMenu;

    ObjectController *m_controller;

    WorldObserver *m_worldObserver;

    QList<ByteArray> m_backupScenes;

    QMap<uint32_t, AssetConverterSettings *> m_sceneSettings;

    QMap<QString, QLineEdit *> m_snapSettings;

    QList<QAction *> m_objectActions;
    QList<QAction *> m_prefabActions;
    QAction *m_activeSceneAction;

    VariantMap m_isolationBackState;

    AssetConverterSettings *m_isolationSettings;

    World *m_isolationWorld;
    Scene *m_isolationScene;

    QToolButton *m_componentButton;
};

#endif // SCENECOMPOSER_H
