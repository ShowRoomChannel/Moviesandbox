

#include "switchCameraNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"

SwitchCameraNode::SwitchCameraNode(){
    name="SwitchCameraNode";
    cameraActor=NULL;

    listType.push_back("15PickWorldButton");
    listName.push_back("cameraActor");
    listProp.push_back("NULL");

    color=Vector4f(0.5,0.5,0.5,1.0);
    registerProperties();
    controller=input->controller;
    }

SwitchCameraNode::~SwitchCameraNode(){}

void SwitchCameraNode::registerProperties(){

Node::registerProperties();
createMemberID("CAMERAACTOR",&cameraActor,this);
}

void SwitchCameraNode::start(){
    if (cameraActor){
        cameraActor->drawType=DRAW_NULL;
        controller->switchTool(TOOL_NAV);
    }
    Node::start();
}

void SwitchCameraNode::stop(){

    if (cameraActor){
        controller->switchTool(TOOL_SELECT);
        cameraActor->drawType=DRAW_VBOMESH;
        Node::stop();
        input->controller->controlledActor=input->controller;
    }
}

void SwitchCameraNode::execute(){
    input->controller->controlledActor=cameraActor;
    nextNode();
}


void SwitchCameraNode::trigger(Actor * other){

    if (other==listButton[0] && input->worldTarget){
        cameraActor=input->worldTarget;
        cout << "SwitchCameraNode triggered!!!" << cameraActor->name << endl;
        }
    }

void SwitchCameraNode::update(double deltaTime){
    Node::update(deltaTime);
    }

void SwitchCameraNode::create(){renderer->addNode(this);}