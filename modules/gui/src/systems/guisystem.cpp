#include "systems/guisystem.h"

#include <components/actor.h>

#include <resources/sprite.h>

#include <commandbuffer.h>

#include "components/recttransform.h"

#include "components/widget.h"
#include "components/image.h"
#include "components/label.h"
#include "components/button.h"
#include "components/switch.h"
#include "components/progressbar.h"
#include "components/frame.h"
#include "components/textinput.h"

GuiSystem::GuiSystem() :
        System() {

    RectTransform::registerClassFactory(this);

    Widget::registerClassFactory(this);
    Image::registerClassFactory(this);
    Frame::registerClassFactory(this);
    Label::registerClassFactory(this);

    AbstractButton::registerClassFactory(this);
    Button::registerClassFactory(this);
    Switch::registerClassFactory(this);

    ProgressBar::registerClassFactory(this);

    TextInput::registerClassFactory(this);
}

GuiSystem::~GuiSystem() {
    TextInput::unregisterClassFactory(this);

    ProgressBar::unregisterClassFactory(this);

    Switch::unregisterClassFactory(this);
    Button::unregisterClassFactory(this);
    AbstractButton::unregisterClassFactory(this);

    Label::unregisterClassFactory(this);
    Image::unregisterClassFactory(this);
    Frame::unregisterClassFactory(this);
    Widget::unregisterClassFactory(this);

    RectTransform::unregisterClassFactory(this);
}

bool GuiSystem::init() {
    return true;
}

const char *GuiSystem::name() const {
    return "ThunderGui";
}

void GuiSystem::update(SceneGraph *) {

}

int GuiSystem::threadPolicy() const {
    return Main;
}

Object *GuiSystem::instantiateObject(const MetaObject *meta, const string &name, Object *parent) {
    Object *result = System::instantiateObject(meta, name, parent);
    Actor *actor = dynamic_cast<Actor *>(parent);
    if(actor) {
        actor->setLayers(CommandBuffer::UI | CommandBuffer::RAYCAST);
    }
    return result;
}

void GuiSystem::composeComponent(Component *component) const {
    Renderable *widget = dynamic_cast<Renderable *>(component);
    if(widget) {
        widget->composeComponent();
    }
}
