

#include "setPropertyNode.h"
#include "control.h"
#include "renderer.h"

SetPropertyNode::SetPropertyNode(){

    name="SetProp";
    applyTo=NULL;
    propertyToSet="";
    propValue="";

    listType.push_back("15PickWorldButton");
    listName.push_back("applyTo");
    listProp.push_back("APPLYTO");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("PropName");
    listProp.push_back("PROPERTYTOSET");
    listIcon.push_back("icon_flat");

    listType.push_back("15TextInputButton");
    listName.push_back("PropValue");
    listProp.push_back("PROPVALUE");

    registerProperties();
}

SetPropertyNode::~SetPropertyNode(){}

void SetPropertyNode::registerProperties(){

Actor::registerProperties();
createMemberID("PROPERTYTOSET",&propertyToSet,this);
createMemberID("PROPVALUE",&propValue,this);
createMemberID("APPLYTO",&applyTo,this);
}

void SetPropertyNode::start(){

Node::start();
}

void SetPropertyNode::stop(){

Node::stop();
}

void SetPropertyNode::execute(){

    //make copy of string, so we don't change the original!
    string s=propValue;
    replace (s.begin(), s.end(),'_',' ');

    if (propertyToSet!="")
        applyTo->memberFromString(&applyTo->property[propertyToSet],s);

    nextNode();
}


void SetPropertyNode::update(double deltaTime){

Node::update(deltaTime);
}

void SetPropertyNode::create(){sceneData->addNode(this);}
