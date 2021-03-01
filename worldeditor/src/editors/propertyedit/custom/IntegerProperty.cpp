#include "IntegerProperty.h"

#include "../editors/IntegerEdit.h"
#include "../nextobject.h"

IntegerProperty::IntegerProperty(const QString &name, QObject *propertyObject, QObject *parent) :
        Property(name, propertyObject, parent) {

}

QWidget *IntegerProperty::createEditor(QWidget *parent, const QStyleOptionViewItem &option) {
    Q_UNUSED(option)
    m_Editor = new IntegerEdit(parent);
    NextObject *object = dynamic_cast<NextObject *>(m_propertyObject);
    if(object) {
        m_Editor->setDisabled(object->isReadOnly(objectName()));
    }
    connect(m_Editor, SIGNAL(editingFinished()), this, SLOT(onDataChanged()));
    return m_Editor;
}

bool IntegerProperty::setEditorData(QWidget *editor, const QVariant &data) {
    IntegerEdit *e = dynamic_cast<IntegerEdit *>(editor);
    if(e) {
        e->blockSignals(true);
        e->setValue(data.toInt());
        e->blockSignals(false);
        return true;
    }
    return Property::setEditorData(editor, data);
}

QVariant IntegerProperty::editorData(QWidget *editor) {
    IntegerEdit *e = dynamic_cast<IntegerEdit *>(editor);
    if(e) {
        return QVariant(e->value());
    }
    return Property::editorData(editor);
}

void IntegerProperty::onDataChanged() {
    IntegerEdit *e = dynamic_cast<IntegerEdit *>(m_Editor);
    if(e) {
        setValue(QVariant(e->value()));
    }
}

QSize IntegerProperty::sizeHint(const QSize &size) const {
    return QSize(size.width(), 27);
}
