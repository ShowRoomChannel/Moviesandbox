

#include "spawnTool.h"
#include "renderer.h"
#include "input.h"

SpawnTool::SpawnTool(){

    spawnType="13SkeletalActor";
    brushMove=BRUSH_ABSOLUTE;
    brushRot=BRUSH_X;
    bKeepSelection=false;
 }

SpawnTool::~SpawnTool(){}

void SpawnTool::start(){

    MsbTool::start();

    brush=renderer->brush;
    brush->bHidden=false;
    brush->drawType=DRAW_VBOMESH;
    brush->color.a=0.5;

    glutSetCursor(GLUT_CURSOR_INHERIT);
    highlightButton(highlightBtn);
}

void SpawnTool::stop(){

    MsbTool::stop();
    brush->bHidden=true;
    brush->drawType=DRAW_SPRITE;
    brush->color.a=1.0;

    glutSetCursor(GLUT_CURSOR_INHERIT);
    lowlightButton();
}


void SpawnTool::keyReleased(int key){

    MsbTool::keyReleased(key);
    if (key=='.'){
        brushRot++;
        if (brushRot>BRUSH_Z)
            brushRot=BRUSH_X;

        if (brushRot==BRUSH_X)
            renderer->brush->setRotation(brushX,brushY,brushZ);
        if (brushRot==BRUSH_Y)
            renderer->brush->setRotation(brushZ,brushY,brushX);
        if (brushRot==BRUSH_Z)
            renderer->brush->setRotation(brushX,brushZ,brushY);

    }
    if (key=='n'){
        if (brushMove==BRUSH_NORMAL)
            brushMove=BRUSH_ABSOLUTE;
        else
            brushMove=BRUSH_NORMAL;

    }

}

void SpawnTool::mouseReleased(int btn){

    MsbTool::mouseReleased(btn);

    if (btn==MOUSEBTNLEFT){
        renderer->actorInfo[spawnType].actorReference->create();

        Actor* myActor=renderer->actorList.back();


        myActor->setAbsoluteLocation(input->mouse3D);

        postSpawn(myActor);
    }
    //allow Actor selection in spawn tool!
    if (btn==MOUSEBTNRIGHT){
        if (input->worldTarget){

			//right Button creates menu if on selected actor
			if (btn==MOUSEBTNRIGHT && input->worldTarget->name!="ground"){
			    for (int i=0;i<(int)input->selectedActors.size();i++)
                    if (input->worldTarget==input->selectedActors[i])
                        input->createActorMenu();
			}
						//any Button selects!

			if (!bKeepSelection) selectActors(btn, input->worldTarget);

        }
        else{
            input->deselectActors();
        }
    }
}

void SpawnTool::selectActors(int btn, Actor* other){

    //don't do anything to selection if we're just finishing a move or rotate
    if (input->actorMenu && input->actorMenu->listButton.size()>0)
        return;

    //can select ground from layerInspector!
    if (other->name=="ground" && input->hudTarget==NULL){
        input->deselectActors();
        return;
    }

    if (input->bShiftDown){                                                                //adding to selection
        bool foundInSelection=false;
        for (int i=0;i<(int)input->selectedActors.size();i++){
            if (other==input->selectedActors[i])
                foundInSelection=true;                                              //only add if we're not already in selection
            }
        if (!foundInSelection){
            if (other->groupID!="NULL" && !input->bCtrlDown){                        //if we're part of a group, add the whole group
                for (int i=0;i<(int)renderer->actorList.size();i++){                //unless we hold down ctrl, then only add this one!
                    if (renderer->actorList[i]->groupID==other->groupID){
                        renderer->actorList[i]->bSelected=true;
                        input->selectedActors.push_back(renderer->actorList[i]);
                        }
                    }
                }
            else{
                other->bSelected=true;
                input->selectedActors.push_back(other);                              //no CTRL or no group, but SHIFT down, so should add
                }
            }
    }else{
        bool foundInSelection=false;
        for (int i=0;i<(int)input->selectedActors.size();i++){
            if (other==input->selectedActors[i])
                foundInSelection=true;                                              //check if we're not already in selection
        }
        if (!foundInSelection && btn==MOUSEBTNRIGHT){                           //not in selection and clicked right
            input->deselectActors();
            other->bSelected=true;
            input->selectedActors.push_back(other);
        }
    }
}

void SpawnTool::update(double deltaTime){

    MsbTool::update(deltaTime);

    renderer->brush->setLocation(input->mouse3D);

    if (brushMove==BRUSH_ABSOLUTE){
        brushX=Vector3f(1,0,0);
        brushY=Vector3f(0,1,0);
        brushZ=Vector3f(0,0,1);
    }

    if (brushMove==BRUSH_NORMAL){
        if (input->worldNormal!=Vector3f(0,1,0)){
            brushZ=input->worldNormal;
            brushZ.normalize();
            brushX=brushZ.crossProduct(Vector3f(0,1,0));
            brushX.normalize();
            brushY= brushZ.crossProduct(brushX);
            brushY.normalize();
        }
        else if (input->worldNormal!=Vector3f(0,0,0)){
            brushY= input->worldNormal;
            brushY.normalize();
            brushX= brushY.crossProduct(Vector3f(0,0,1));
            brushX.normalize();
            brushZ= brushY.crossProduct(brushX);
            brushZ.normalize();
        }else{
            input->worldNormal=Vector3f(0,1,0);
            brushY= input->worldNormal;
            brushY.normalize();
            brushX= brushY.crossProduct(Vector3f(0,0,1));
            brushX.normalize();
            brushZ= brushY.crossProduct(brushX);
            brushZ.normalize();
        }
    }

    if (brushRot==BRUSH_X)
        renderer->brush->setRotation(brushX,brushY,brushZ);
    if (brushRot==BRUSH_Y)
        renderer->brush->setRotation(brushZ,brushY,brushX);
    if (brushRot==BRUSH_Z)
        renderer->brush->setRotation(brushX,brushZ,brushY);

}


void SpawnTool::postSpawn(Actor* myActor){


    cout << "Spawning..." << brush->vboMeshID << endl;
    //assign vboMeshID from Brush to new Actor
    myActor->vboMeshID=brush->vboMeshID;
    if (renderer->vboList[brush->vboMeshID]->bIsSkeletal)
        myActor->sceneShaderID="skeletal";

    myActor->setRotation(brush->rotation);
    myActor->setup();
}

