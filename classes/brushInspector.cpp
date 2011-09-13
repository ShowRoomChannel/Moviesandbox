

#include "brushInspector.h"
#include "renderer.h"
#include "input.h"
#include "colorPickButton.h"
#include "sliderButton.h"


BrushInspector::BrushInspector(){

level=0;
currentSwatch=0;
numSwatches=5;

registerProperties();
}

BrushInspector::~BrushInspector(){}


void BrushInspector::setup(){
    Inspector::setup();
}


void BrushInspector::createInspectorButtons(){


    ColorPickButton* colorPick= new ColorPickButton;
    colorPick->name="pick Color";
    colorPick->bPermanent=true;
    colorPick->level=level+1;
    colorPick->setLocation(location + Vector3f(scale.x+5 ,5,0));

    colorPick->initialLocation=colorPick->location;
    colorPick->parent=this;

    sceneData->buttonList.push_back(colorPick);
    inspectorButtons.push_back(colorPick);

    for (int i=0;i<numSwatches;i++){
        AssignButton* showColor = new AssignButton;
        showColor->name="show color";
        showColor->parent=this;
        showColor->bPermanent=true;
        showColor->level=level+1;
        showColor->setLocation(location+Vector3f(35 + i*34,160,0));
        showColor->initialLocation=showColor->location;
        showColor->scale=Vector3f(32,32,1);
        showColor->textureID="icon_flat";
        showColor->color=Vector4f(1,1,1,1);


        sceneData->buttonList.push_back(showColor);
        inspectorButtons.push_back(showColor);
        colorSwatches.push_back(showColor);
    }

    colorSwatches[0]->textureID="icon_base";

    SliderButton* brushScaleBtn= new SliderButton;
    brushScaleBtn->name="brush scale";
    brushScaleBtn->parent=this;
    brushScaleBtn->bPermanent=true;
    brushScaleBtn->level=level+1;
    brushScaleBtn->bDrawName=true;
    brushScaleBtn->setLocation(location+Vector3f(35,200,0));
    brushScaleBtn->initialLocation=brushScaleBtn->location;
    brushScaleBtn->scale=Vector3f(256,15,1);
    brushScaleBtn->textureID="icon_flat";
    brushScaleBtn->bVertical=false;

    sceneData->buttonList.push_back(brushScaleBtn);
    inspectorButtons.push_back(brushScaleBtn);

    SliderButton* brushIntensityBtn= new SliderButton;
    brushIntensityBtn->name="brush intensity";
    brushIntensityBtn->parent=this;
    brushIntensityBtn->bPermanent=true;
    brushIntensityBtn->level=level+1;
    brushIntensityBtn->bDrawName=true;
    brushIntensityBtn->setLocation(location+Vector3f(35,220,0));
    brushIntensityBtn->initialLocation=brushIntensityBtn->location;
    brushIntensityBtn->scale=Vector3f(256,15,1);
    brushIntensityBtn->textureID="icon_flat";
    brushIntensityBtn->bVertical=false;

    sceneData->buttonList.push_back(brushIntensityBtn);
    inspectorButtons.push_back(brushIntensityBtn);


}

void BrushInspector::refreshList(){

/*
    if (bOpen && listButton.size()==0){
        cout << "assembling brushinspector..." << endl;
        assembleList();
    }
*/

}

void BrushInspector::assembleList(){


    ///Grid specific buttons

/*
        AssignButton* gridAssign = new AssignButton;
        gridAssign->name="Grid Plane XY";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(35,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->textureID="icon_gridXY";
        gridAssign->color=Vector4f(1,1,1,1);

        sceneData->buttonList.push_back(gridAssign);
        listButton.push_back(gridAssign);

        gridAssign = new AssignButton;
        gridAssign->name="Grid Plane ZY";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(70,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->textureID="icon_gridZY";
        gridAssign->color=Vector4f(1,1,1,1);

        sceneData->buttonList.push_back(gridAssign);
        listButton.push_back(gridAssign);

        gridAssign = new AssignButton;
        gridAssign->name="Grid Plane XZ";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(105,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->textureID="icon_gridXZ";
        gridAssign->color=Vector4f(1,1,1,1);

        sceneData->buttonList.push_back(gridAssign);
        listButton.push_back(gridAssign);

        gridAssign = new AssignButton;
        gridAssign->name="flip Grid";
        gridAssign->parent=this;
        gridAssign->bPermanent=true;
        gridAssign->level=level+1;
        gridAssign->setLocation(location+Vector3f(140,350,0));
        gridAssign->initialLocation=gridAssign->location;
        gridAssign->scale=Vector3f(32,32,1);
        gridAssign->textureID="icon_gridZY";
        gridAssign->color=Vector4f(1,1,1,1);

        sceneData->buttonList.push_back(gridAssign);
        listButton.push_back(gridAssign);
*/
}

void BrushInspector::trigger(MsbObject* other){

    if (other->name=="Grid Plane XY"){
        sceneData->gridTool->gridRot=GRID_X-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="Grid Plane ZY"){
        sceneData->gridTool->gridRot=GRID_Y-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="Grid Plane XZ"){
        sceneData->gridTool->gridRot=GRID_Z-1;
        sceneData->gridTool->keyReleased('.');
    }

    if (other->name=="flip Grid"){

        if (sceneData->gridTool->gridRot==GRID_Z)
            sceneData->grid->addRotation(180,Vector3f(1,0,0));
        else
            sceneData->grid->addRotation(180,Vector3f(0,1,0));

    }

    if (other->name=="brush scale"){
        sceneData->brush->scale.x= pow(0.4+ ((SliderButton*)other)->sliderValue,2.0) ;
    }

    if (other->name=="brush intensity"){
        sceneData->brush->intensity=min(1.0, 0.01 + ((SliderButton*)other)->sliderValue);
    }

    if (other->name=="pick Color"){
        cout << "triggered from pickColor!" << endl;
        color=Vector4f( max(min(color.r, 1.0f),0.0f),max(min(color.g, 1.0f),0.0f), max(min(color.b, 1.0f),0.0f), max(min(color.a, 1.0f),0.0f) );
        colorSwatches[currentSwatch]->color=color;
        sceneData->brush->setColor(color);
        color=Vector4f(1.0,0.6,0.1,1.0);
    }

    if (other->name=="show color"){

        if (sceneData->controller->tool==TOOL_SELECT){
            if (sceneData->selectedActors.size()>0){
                    for (int i=0;i<(int)sceneData->selectedActors.size();i++){
                        sceneData->selectedActors[i]->color=other->color;
                    }
            }
        }else if (sceneData->controller->tool==TOOL_DRAW){
            sceneData->brush->color=other->color;
        }

        for (int i=0;i<numSwatches;i++){
            colorSwatches[i]->textureID="icon_flat";
            if (other==colorSwatches[i])
                currentSwatch=i;
        }
        ((Actor*)other)->textureID="icon_base";
    }

}

void BrushInspector::create(){sceneData->addButton(this);}
