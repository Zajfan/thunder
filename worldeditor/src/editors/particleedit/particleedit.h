#ifndef PARTICLEEDIT_H
#define PARTICLEEDIT_H

#include <QMainWindow>
#include <stdint.h>

#include "editors/scenecomposer/documentmodel.h"

class QSettings;

class Engine;
class Actor;
class ParticleRender;

class Viewport;

class EffectConverter;

namespace Ui {
    class ParticleEdit;
}

class ParticleEdit : public QMainWindow, public IAssetEditor {
    Q_OBJECT

public:
    ParticleEdit();
    ~ParticleEdit();

    void readSettings();
    void writeSettings();

    void loadAsset(IConverterSettings *settings) override;

signals:
    void templateUpdate();

private slots:
    void onGLInit();

    void onUpdateTemplate(bool update = true);

    void onNodeSelected(void *node);
    void onNodeDeleted();

    void onEmitterSelected(QString emitter);
    void onEmitterCreated();
    void onEmitterDeleted(QString name);

    void onFunctionSelected(QString emitter, QString function);
    void onFunctionCreated(QString emitter, QString function);
    void onFunctionDeleted(QString emitter, QString function);

    void onToolWindowActionToggled(bool checked);

    void onToolWindowVisibilityChanged(QWidget *toolWindow, bool visible);

    void on_actionSave_triggered();

private:
    void closeEvent(QCloseEvent *event) override;

    void timerEvent(QTimerEvent *) override;

    bool isModified() const override;

    bool m_Modified;

    Ui::ParticleEdit *ui;

    QObject *m_pEditor;

    Actor *m_pEffect;

    QString m_Path;

    EffectConverter *m_pBuilder;

    ParticleRender *m_pRender;
};

#endif // PARTICLEEDIT_H
