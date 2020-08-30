#ifndef ANIMATIONEDIT_H
#define ANIMATIONEDIT_H

#include <QMainWindow>

#include "editors/scenecomposer/documentmodel.h"

class AbstractSchemeModel;
class AnimationStateMachine;

namespace Ui {
    class AnimationEdit;
}

class AnimationEdit : public QMainWindow, public IAssetEditor {
    Q_OBJECT

public:
    explicit AnimationEdit(Engine *engine);
    ~AnimationEdit();

    void readSettings();
    void writeSettings();

    void loadAsset(IConverterSettings *settings);

signals:
    void templateUpdate();

private slots:
    void on_actionSave_triggered();

    void onNodesSelected(const QVariant &);

    void onUpdateTemplate(bool update = true);

    void onToolWindowActionToggled(bool checked);

    void onToolWindowVisibilityChanged(QWidget *toolWindow, bool visible);

private:
    void closeEvent(QCloseEvent *event);

    Ui::AnimationEdit *ui;

    AbstractSchemeModel *m_pBuilder;

    AnimationStateMachine *m_pMachine;

    QString m_Path;

    QAction *m_pUndo;
    QAction *m_pRedo;
};

#endif // ANIMATIONEDIT_H
