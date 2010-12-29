

#include "inspector.h"
#include "renderer.h"
#include "input.h"
#include "inspectorManager.h"

Inspector::Inspector(){

    bOpen=false;
    bPermanent=true;
    initialLocation=location;
    tabWidth=350;
    tabHeight=0;
    listHeight=64;
    listWidth=64;
    listDisplayMode=4;
    listColumns=1;
    level=0;

    textureID="icon_ball";

    backgroundButton=NULL;

    listOffset=32;

    registerProperties();
}

Inspector::~Inspector(){}


void Inspector::setup(){

    ListButton::setup();
    backgroundButton=new BasicButton;
    renderer->buttonList.push_back(backgroundButton);
    backgroundButton->sceneShaderID="color";

    if (tabHeight==0){
        backgroundButton->scale.x=350;
        backgroundButton->scale.y=input->screenY-location.y;
        backgroundButton->location.x=location.x+scale.x;
        backgroundButton->location.y=location.y;
    }else{
        backgroundButton->scale.x=input->screenX;
        backgroundButton->scale.y=input->screenY;
        backgroundButton->location.x=location.x;
        backgroundButton->location.y=location.y+scale.y;
    }

    backgroundButton->level=level;
    backgroundButton->bPermanent=true;
    backgroundButton->name=name;
    backgroundButton->color=Vector4f(0.4,0.4,0.4,1.0);
    backgroundButton->setLocation(backgroundButton->location);

    assembleList();

    //create inspectorButtons
    createInspectorButtons();

    myManager=input->inspectorManager;

}

void Inspector::update(double deltaTime){

    refreshList();

    //check if we need scrollBar and make one if we do
	if (scrollBar){
		if ((int)listButton.size()>maxListItems){
			trigger(scrollBar);
			placeScrollBar();
			scrollBar->bHidden=!bOpen;
		}else {
			scrollBar->remove();
			scrollBar=NULL;
		}

	}
	else{
		if ((int)listButton.size()>maxListItems)
			createScrollBar();
	}
}

void Inspector::createInspectorButtons(){

}

void Inspector::refreshList(){

}

void Inspector::drawPlane(){

    BasicButton::drawPlane();

    renderer->setupShading("color");

   /*
    if (bOpen){
        if (tabHeight==0)
            renderer->drawPlane(scale.x ,0.0, renderer->screenX,renderer->screenY, Vector4f(0.4,0.4,0.4, 1.0));
        else
            renderer->drawPlane(0.0 ,scale.y, renderer->screenX,renderer->screenY, Vector4f(0.4,0.4,0.4, 1.0));
    }
*/

}

void Inspector::mouseOver(){

ListButton::mouseOver();
}

void Inspector::mouseDrag(){}
void Inspector::finishDrag(){}

void Inspector::clickedLeft(){

    if (!bOpen){

        bOpen=true;

        myManager->inspectorOpened(this);

        initialLocation=location;
        setLocation(location-Vector3f(tabWidth, tabHeight, 0) );
        if (tabHeight==0)
            backgroundButton->setLocation(location + Vector3f(scale.x,0,0));
        else
            backgroundButton->setLocation(location + Vector3f(0,scale.y,0));

        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=false;
            listButton[i]->setLocation( listButton[i]->location - Vector3f( tabWidth, tabHeight, 0 ) );
            listButton[i]->initialLocation-=Vector3f( tabWidth, tabHeight, 0 );
            }
        for (int i=0;i<(int)inspectorButtons.size();i++){
            inspectorButtons[i]->bHidden=false;
            inspectorButtons[i]->setLocation( inspectorButtons[i]->location - Vector3f( tabWidth, tabHeight, 0 ) );
            inspectorButtons[i]->initialLocation-=Vector3f( tabWidth, tabHeight, 0 );
        }
    }
    else{

        for (int i=0;i<(int)listButton.size();i++){
            listButton[i]->bHidden=true;
            listButton[i]->setLocation( listButton[i]->location + Vector3f( tabWidth, tabHeight, 0 ) );
            listButton[i]->initialLocation+=Vector3f( tabWidth, tabHeight, 0 );
        }
        for (int i=0;i<(int)inspectorButtons.size();i++){
            inspectorButtons[i]->bHidden=true;
            inspectorButtons[i]->setLocation( inspectorButtons[i]->location + Vector3f( tabWidth, tabHeight, 0 ) );
            inspectorButtons[i]->initialLocation+=Vector3f( tabWidth, tabHeight, 0 );
        }
        setLocation(initialLocation);
        if (tabHeight==0)
            backgroundButton->setLocation(location + Vector3f(scale.x,0,0));
        else
            backgroundButton->setLocation(location + Vector3f(0,scale.y,0));
        bOpen=false;

        myManager->inspectorClosed(this);
    }

    cout << "clicked on Inspecta!" << endl;
}

void Inspector::assembleList(){

        cout << "creating list..." << endl;
}

void Inspector::clickedRight(){}

void Inspector::focusClick(){

ListButton::focusClick();
}

void Inspector::placeButton(int buttonNumber, int drawPosition){

    ListButton::placeButton(buttonNumber,drawPosition);
    listButton[buttonNumber]->setLocation(listButton[buttonNumber]->location + Vector3f( 0, listOffset, 0 ) );
}

void Inspector::placeScrollBar(){

    scrollBar->setLocation(Vector3f(renderer->screenX-50, location.y+scale.y +2, 0) );
}

void Inspector::deselect(int depth){

//Button::deselect(depth);
}

void Inspector::create(){renderer->addButton(this);}