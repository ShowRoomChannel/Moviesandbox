

#include "gridTool.h"
#include "renderer.h"
#include "input.h"

GridTool::GridTool(){

    gridMove=GRID_ABSOLUTE;
    gridRot=GRID_X;
    bOverrideOldState=false;
}

GridTool::~GridTool(){}

void GridTool::setup(){

  MsbTool::setup();
}

void GridTool::start(){

    MsbTool::start();
    sceneData->grid->bPickable=false;
    sceneData->grid->bHidden=false;
    myBtn->color=Vector4f(1.0,0.0,0.0,1.0);

}

void GridTool::stop(){

    MsbTool::stop();
    gridMove=GRID_ABSOLUTE;
    sceneData->grid->bPickable=true;
    myBtn->color=Vector4f(1.0,1.0,1.0,1.0);
}

void GridTool::keyPressed(int key){

    MsbTool::keyPressed(key);
    if (key=='r' || key=='R' || key==18){
        gridMove=GRID_ROTATE;
    }
}

void GridTool::keyReleased(int key){

    gridMove=GRID_ABSOLUTE;

    MsbTool::keyReleased(key);
    if (key=='.'){
        gridRot++;
        if (gridRot>2)
            gridRot=0;

        if (gridRot==GRID_X)
            sceneData->grid->setRotation(-gridX,gridY,-gridZ);
        if (gridRot==GRID_Y)
            sceneData->grid->setRotation(-gridZ,gridY,gridX);
        if (gridRot==GRID_Z)
            sceneData->grid->setRotation(gridX,gridZ,gridY);

    }

    if (key=='n')
        gridMove=GRID_NORMAL;

}

void GridTool::mousePressed(int btn){

    MsbTool::mousePressed(btn);
}
void GridTool::mouseReleased(int btn){
    MsbTool::mouseReleased(btn);
}

void GridTool::mouseDragged(int btn){
    MsbTool::mouseDragged(btn);
}


void GridTool::update(double deltaTime){

    MsbTool::update(deltaTime);


    if (!bPressLeft || input->hudTarget)
        return;

    if (gridMove==GRID_ROTATE){
        if (bPressLeft)
            rotateGrid(deltaTime);
        return;
    }

    sceneData->grid->bPickable=false;

    if (gridMove==GRID_ABSOLUTE){
        sceneData->grid->setLocation(input->mouse3D);
        gridX=Vector3f(-1,0,0);
        gridY=Vector3f(0,1,0);
        gridZ=Vector3f(0,0,-1);
    }

    if (gridMove==GRID_NORMAL){
        sceneData->grid->setLocation(input->mouse3D);
        if (input->worldNormal!=Vector3f(0,1,0)){
            gridZ=input->worldNormal;
            gridZ.normalize();
            gridX=gridZ.crossProduct(Vector3f(0,1,0));
            gridX.normalize();
            gridY= gridZ.crossProduct(gridX);
            gridY.normalize();
        }
        else if (input->worldNormal!=Vector3f(0,0,0)){
            gridY= input->worldNormal;
            gridY.normalize();
            gridX= gridY.crossProduct(Vector3f(0,0,1));
            gridX.normalize();
            gridZ= gridY.crossProduct(gridX);
            gridZ.normalize();
        }else{
            input->worldNormal=Vector3f(0,1,0);
            gridY= input->worldNormal;
            gridY.normalize();
            gridX= gridY.crossProduct(Vector3f(0,0,1));
            gridX.normalize();
            gridZ= gridY.crossProduct(gridX);
            gridZ.normalize();
        }
    }

}

void GridTool::rotateGrid(double deltaTime){

        Vector3f axis;

        float amount=input->mouseVector.x * sceneData->mouseSensitivity;

        Vector3f xA,yA,zA;
        sceneData->grid->getAxis(&xA,&yA,&zA,sceneData->grid->transformMatrix);

        if (input->bCtrlDown)
            axis=zA;
        else if (input->bShiftDown)
            axis=xA;
        else
            axis=yA;

        sceneData->grid->addRotation(amount, axis);
        gridX=sceneData->grid->xAxis;
        gridY=sceneData->grid->yAxis;
        gridZ=sceneData->grid->zAxis;

        input->bConfineMouse=true;

}

