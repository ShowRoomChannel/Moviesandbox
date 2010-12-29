

#include "inspectorManager.h"
#include "renderer.h"

InspectorManager::InspectorManager(){

lastOpenInspector=NULL;

registerProperties();
}

InspectorManager::~InspectorManager(){}

void InspectorManager::registerProperties(){

  Actor::registerProperties();
}

void InspectorManager::setup(){

  Actor::setup();
  //automatically find all inspectors and generate a list

  for (int i=0;i<(int)renderer->buttonList.size();i++){
    Inspector* myInspector = dynamic_cast<Inspector*>(renderer->buttonList[i]);
    if (myInspector)
        inspectors.push_back(myInspector);
  }
}

void InspectorManager::trigger(Actor * other){}

void InspectorManager::inspectorOpened(Inspector * other){

    if (lastOpenInspector)
        lastOpenInspector->clickedLeft();

    lastOpenInspector=other;
}

void InspectorManager::inspectorClosed(Inspector * other){

    lastOpenInspector=NULL;

}

void InspectorManager::update(double deltaTime){

  Actor::update(deltaTime);
}

void InspectorManager::create(){renderer->addActor(this);}