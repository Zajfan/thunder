#include "selecttool.h"

#include <components/actor.h>
#include <components/transform.h>

#include <editor/handles.h>

#include "objectctrl.h"
#include "undomanager.h"

SelectTool::SelectTool(ObjectCtrl *controller, SelectMap &selection) :
    EditorTool(selection),
    m_pController(controller) {

}

void SelectTool::startDrag() {
    // Save params
    for(auto &it : m_Selected) {
        Transform *t = it.object->transform();
        it.position = t->position();
        it.scale    = t->scale();
        it.euler    = t->rotation();
    }
    m_Position = objectPosition();
    m_SavedWorld = m_World;
}

QString SelectTool::icon() const {
    return ":/Images/editor/Select.png";
}

QString SelectTool::name() const {
    return "selectTool";
}
