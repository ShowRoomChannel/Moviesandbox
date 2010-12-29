#include "control.h"
#include "input.h"
#include "physicsActor.h"
#include "calligraphyFilter.h"
#include "navTool.h"
#include "particleSelectTool.h"


/*

Constructor

****************************/

bool Control::bRunning=false;

vector<Node *> Control::nodeTree;               //currently active nodes

Control::Control(){

tool=TOOL_SELECT;
oldTool=TOOL_SELECT;
controlledActor=this;
moveMode=FLYINGCAMERA;                        //as opposed to walking
bDrawOrientation=false;
bPickable=false;
bHidden=true;
name="Control!";


//eventTrigger="NULL";
location=Vector3f(0,2,-10);
}

void Control::setup(){

    Actor::setup();
    myTools.push_back(new SelectTool);
    myTools.push_back(new NavTool);
    myTools.push_back(new GridTool);
    myTools.push_back(new SpawnTool);
    myTools.push_back(new DrawTool);
    myTools.push_back(new DrawTool);
    myTools.push_back(new ParticleSelectTool);
    myTools.push_back(new BoneTool);
    myTools.push_back(new SkinTool);


    for (int i=0;i<(int)myTools.size();i++){
        myTools[i]->setup();
    }
    currentTool=myTools[TOOL_SELECT];

    oldTool=TOOL_SELECT;
    tool=TOOL_SELECT;

    ((DrawTool*)myTools[5])->filters.push_back(new CalligraphyFilter((DrawTool*)myTools.back()));
    ((DrawTool*)myTools[5])->highlightBtn="Draw Calligraphy";
}

void Control::update(double deltaTime){


    currentTool->update(deltaTime);

	//handle event triggering!
    for (int i=0;i<(int)input->eventTrigger.size();i++){
        eventTrigger.push_back(input->eventTrigger[i]);
    }
    input->eventTrigger.clear();

    //do the run thing
    if (bRunning){
        updateNodes();
    }

    //event Trigger
    eventTrigger.clear();
    Actor::update(deltaTime);

	//always update navTool!
	((NavTool*)myTools[TOOL_NAV])->processMove(0.0);

}

//for Tool-Button Callbacks
void Control::trigger(Actor* other){

    cout << "triggered controller!" << endl;

    if (other->name=="select"){
            switchTool(TOOL_SELECT);
    }


    if (other->name=="navigate"){
            switchTool(TOOL_NAV);
    }

    if (other->name=="grid"){
            switchTool(TOOL_GRID);
    }

}

void Control::updateNodes(){

for (unsigned int i=0;i<nodeTree.size();i++)
  if (nodeTree[i])
    nodeTree[i]->execute();
}

void Control::startMovie(){

bRunning=!bRunning;

if (bRunning)
	start();
else
	stop();
}


void Control::switchTool(int newTool){

    //if we switch to the same state for the second time, switch back to the old one!
    //unless it's select. Because that would create headaches...
    if (tool==newTool)
        newTool=oldTool;

    //remember last, if tool wants us to
    if (currentTool->bOverrideOldState)
        oldTool=tool;

    //set current
    tool=newTool;

    //stop current tool
    currentTool->stop();

    //switch to new tool
    currentTool=myTools[newTool];

    //start new tool
    currentTool->start();

}


void Control::start(){

cout << "starting simulation..." << endl;

    //setup Actors
    for (uint i=0;i<renderer->actorList.size();i++){
      renderer->actorList[i]->initialTransform=renderer->actorList[i]->transformMatrix;
      renderer->actorList[i]->start();
    }


    //play Timeline when no nodes are present
    if (renderer->nodeList.size()==0){
        timeline->playTimelines();
    }

    //- setup nodes
    for (uint i=0;i<renderer->nodeList.size();i++)
      renderer->nodeList[i]->start();

    //start node script
    cout << "number of trees: " << nodeTree.size() << endl;

    for (uint i=0;i<nodeTree.size();i++)
      nodeTree[i]->execute();

    //- enable physics
    //todo: disabled for debugging!
    renderer->bUpdatePhysics=true;
}

void Control::stop(){

cout << "stopping simulation..." << endl;

    //- disable physics
    renderer->bUpdatePhysics=false;

    //- clean up nodes
    for (unsigned int i=0;i<renderer->nodeList.size();i++)
      renderer->nodeList[i]->stop();

    nodeTree.clear();

    for (uint i=0;i<renderer->actorList.size();i++){
      PhysicsActor * phys = dynamic_cast<PhysicsActor*>(renderer->actorList[i]);
      if (!phys){
        renderer->actorList[i]->transformMatrix=renderer->actorList[i]->initialTransform;
        renderer->actorList[i]->baseMatrix=calcMatrix(renderer->actorList[i]);
        renderer->actorList[i]->matrixToVectors();

        renderer->actorList[i]->stop();
      }
    }

}



