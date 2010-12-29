

#include "boneWidget.h"
#include "renderer.h"
#include "input.h"
#include "skeletalDrawing.h"
#include "brush.h"
#include "spriteMeshLoader.h"

BoneWidget::BoneWidget(){

name="boneWidget";
tooltip="bones";

bWidgetOpen=false;
listColor=Vector4f(0.8,0.8,0.8,1.0);
listDisplayMode=0;
color=listColor;
highlightColor=Vector4f(0.8,0.0,0.0,1.0);
}

BoneWidget::~BoneWidget(){}


void BoneWidget::openWidget(){

    widgetLocation=Vector3f(location.x,location.y,0);

    //switch to create bone mode
    input->controller->switchTool(TOOL_BONE);
    //unless our drawing is already boned (tee-he-he), then we might want to switch to skinning mode!
    if (input->specialSelected && renderer->vboList[input->specialSelected->vboMeshID]->bIsSkeletal)
        input->controller->switchTool(TOOL_SKIN);
}

void BoneWidget::closeWidget(){

    input->deselectActors();

    if (renderer->brush->drawing){
        input->makeWarningPopUp("saving drawing...",this);
        renderer->draw();
        input->controller->myTools[TOOL_BONE]->save();
        renderer->brush->drawing->drawType=DRAW_VBOMESH;
        renderer->brush->drawing->reset();
        input->staticButton->focusClick();
        input->bTextInput=false;
    }


    input->controller->switchTool(TOOL_SELECT);
    color=COLOR_WHITE;

}

void BoneWidget::trigger(Actor* other){


	SkeletalActor* skel=renderer->brush->drawing;
	if (!skel) return;


    if (other->name=="Create Bone (b)"){
        input->controller->switchTool(TOOL_BONE);
    }

    if (other->name=="Paint Weights"){
        if (renderer->brush->drawing){
            input->makeWarningPopUp("saving drawing...",this);
            renderer->draw();
            input->controller->myTools[TOOL_BONE]->save();
            renderer->brush->drawing->drawType=DRAW_VBOMESH;
            renderer->brush->drawing->reset();
            input->staticButton->focusClick();
            input->bTextInput=false;
        }
        input->controller->switchTool(TOOL_SKIN);
    }


	if (other->name=="Load Bones"){

		cout << "loading bones..." << endl;

		string meshID;
		string filename=input->openFileDialog();
		//check for file ending
		size_t found, appendix;
		appendix=filename.find(".spriteMesh");
		if (appendix!=string::npos){
			//generate meshID
			found=filename.find_last_of(DIRECTORY_SEPARATION);
			found+=1;
			meshID=filename.substr(found, appendix-found);
			cout << "found following meshID:" << meshID << endl;
		}else{
			return;
		}
		//resetting bones of drawing...
		string oldMeshID=renderer->brush->drawing->vboMeshID;

		renderer->brush->drawing->vboMeshID=meshID;
		renderer->brush->drawing->reset();
		renderer->brush->drawing->vboMeshID=oldMeshID;

		//set to skeletal if loaded mesh was skeletal too!
		if (renderer->vboList[meshID]->bIsSkeletal)
			renderer->vboList[oldMeshID]->bIsSkeletal=true;

	}

   if (other->name=="save"){
        input->controller->currentTool->save();
    }

    if (other->name=="save As..."){
        cout << "creating VBO..." << endl;
        renderer->spriteMeshLoader->saveSpriteMesh("resources/meshes/"+input->inputText+".spriteMesh",(SkeletalDrawing*)(renderer->brush->drawing));
        renderer->spriteMeshLoader->loadSpriteMesh("resources/meshes/"+input->inputText+".spriteMesh",input->inputText);

		renderer->brush->drawing->name=input->inputText;
		renderer->brush->drawing->vboMeshID=input->inputText;

        TiXmlElement * newMesh= new TiXmlElement("SpriteMesh");
        newMesh->SetAttribute("meshID",input->inputText);
        newMesh->SetAttribute("meshFilename","resources/meshes/"+input->inputText+".spriteMesh");
        input->addToLibrary(newMesh);
        free(newMesh);
    }
}

void BoneWidget::highlight(Actor* other){

        for (int i=0;i<(int)listButton.size();i++)
            if (listButton[i]!=other && listButton[i]->color==highlightColor)
                listButton[i]->color=listColor;

        other->color=highlightColor;
}

void BoneWidget::create(){renderer->addButton(this);}