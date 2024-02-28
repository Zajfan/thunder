#include "components/widget.h"

#include "components/recttransform.h"
#include "components/layout.h"

#include "uisystem.h"

#include <components/actor.h>

#include <commandbuffer.h>
#include <gizmos.h>

Widget *Widget::m_focusWidget = nullptr;

/*!
    \module Gui

    \title Graphical User Interface for Thunder Engine SDK

    \brief Contains classes related to Graphical User Interfaces.
*/

/*!
    \class Widget
    \brief The Widget class is the base class of all user interface objects.
    \inmodule Gui

    The Widget class serves as the base class for all user interface objects, providing basic functionality for handling updates, drawing, and interaction.
    Internal methods are marked as internal and are intended for use within the framework rather than by external code.
*/

Widget::Widget() :
        m_parent(nullptr),
        m_transform(nullptr),
        m_internal(false) {

}

Widget::~Widget() {
    if(m_transform) {
        m_transform->unsubscribe(this);
    }
    static_cast<UiSystem *>(system())->removeWidget(this);
}
/*!
    Sets a textual description of widget style.
*/
string Widget::style() const {
    return m_style;
}
/*!
    Sets a textual description of widget style.
*/
void Widget::setStyle(const string style) {
    m_style = style;
}
/*!
    \internal
    Internal method called to update the widget, including handling layout updates.
*/
void Widget::update() {
    NativeBehaviour::update();

    if(m_transform) {
        Layout *layout = m_transform->layout();
        if(layout) {
            layout->update();
        }
    }
}
/*!
    \internal
    Internal method called to draw the widget using the provided command buffer.
*/
void Widget::draw(CommandBuffer &buffer) {
    if(m_parent == nullptr && m_transform) {
        m_transform->setSize(buffer.viewport());
    }
}
/*!
    Lowers the widget to the bottom of the widget's stack.

    \sa raise()
*/
void Widget::lower() {
    UiSystem *render = static_cast<UiSystem *>(system());

    auto &widgets = render->widgets();
    widgets.remove(this);
    widgets.push_front(this);
}
/*!
    Raises this widget to the top of the widget's stack.

    \sa lower()
*/
void Widget::raise() {
    UiSystem *render = static_cast<UiSystem *>(system());

    auto &widgets = render->widgets();
    widgets.remove(this);
    widgets.push_back(this);
}
/*!
    Callback to respond to changes in the widget's \a size.
*/
void Widget::boundChanged(const Vector2 &size) {
    A_UNUSED(size);
}
/*!
    Returns the parent Widget.
*/
Widget *Widget::parentWidget() {
    return m_parent;
}
/*!
    Returns RectTransform component attached to parent Actor.
*/
RectTransform *Widget::rectTransform() const {
    return m_transform;
}
/*!
    Returns the application widget that has the keyboard input focus, or nullptr if no widget in this application has the focus.
*/
Widget *Widget::focusWidget() {
    return m_focusWidget;
}
/*!
    \internal
    Internal method to set the widget that has the keyboard input focus.
*/
void Widget::setFocusWidget(Widget *widget) {
    m_focusWidget = widget;
}
/*!
    \internal
     Internal method to set the RectTransform component for the widget.
*/
void Widget::setRectTransform(RectTransform *transform) {
    if(m_transform) {
        m_transform->unsubscribe(this);
    }
    m_transform = transform;
    if(m_transform) {
        m_transform->subscribe(this);
    }
}
/*!
    \internal
     Internal method to set the parent of the widget and handle changes.
*/
void Widget::setParent(Object *parent, int32_t position, bool force) {
    NativeBehaviour::setParent(parent, position, force);

    actorParentChanged();
}
/*!
    \internal
    Internal method called when the parent actor of the widget changes.
*/
void Widget::actorParentChanged() {
    Actor *object = actor();
    if(object) {
        setRectTransform(dynamic_cast<RectTransform *>(object->transform()));

        object = dynamic_cast<Actor *>(object->parent());
        if(object) {
            m_parent = static_cast<Widget *>(object->component("Widget"));
        }
    }
}
/*!
    \internal
    Internal method to compose the widget component, creating and setting the RectTransform.
*/
void Widget::composeComponent() {
    setRectTransform(Engine::objectCreate<RectTransform>("RectTransform", actor()));
}
/*!
    \internal
    Internal method to set the system for the widget, adding it to the render system.
*/
void Widget::setSystem(ObjectSystem *system) {
    Object::setSystem(system);

    UiSystem *render = static_cast<UiSystem *>(system);
    render->addWidget(this);
}
/*!
    \internal
    Internal method to draw selected gizmos for the widget, such as a wireframe box.
*/
void Widget::drawGizmosSelected() {
    AABBox box = m_transform->bound();
    Gizmos::drawRectangle(box.center, Vector2(box.extent.x * 2.0f,
                                              box.extent.y * 2.0f), Vector4(0.5f, 1.0f, 0.5f, 1.0f));
}
/*!
    \internal
    Returns true if this widget was defined as internal for the other complex widgets; otherwise returns false.
*/
bool Widget::isInternal() {
    return m_internal;
}
/*!
    \internal
*/
void Widget::makeInternal() {
    m_internal = true;
}