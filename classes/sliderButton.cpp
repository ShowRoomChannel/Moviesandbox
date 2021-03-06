

#include "sliderButton.h"
#include "renderer.h"
#include "input.h"

SliderButton::SliderButton(){

sliderValue=0;
slidePointColor=Vector4f(0.2,0.2,0.2,1.0);
highlightColor=Vector4f(0.8,0.4,0.4,1.0);
bVertical=true;
bFlipValue=false;
tooltip="";
name="SliderButton";
scale.y=80;
bDragable=true;
}


SliderButton::~SliderButton(){}


void SliderButton::setup(){
    BasicButton::setup();

}

void SliderButton::update(double deltaTime){

    BasicButton::update(deltaTime);
    //todo:maybe convert tooltip to string? So we can update easily?

}

void SliderButton::mouseOver(){

    BasicButton::mouseOver();
    bHighlight=true;
}

void SliderButton::mouseDrag(){

input->dragButton=this;

if (bVertical){
    if (bFlipValue) sliderValue=1.0-max(0.0f,min((input->mouseY-location.y)/scale.y, 1.0f)) ;
    else sliderValue=max(0.0f,min((input->mouseY-location.y)/scale.y, 1.0f)) ;
}
else{   //TODO horizontal slider needs min/max stuff
    if (bFlipValue) sliderValue=1.0 - (input->mouseX-location.x)/scale.x;
    else sliderValue=max(0.0f,min((input->mouseX-location.x)/scale.x,1.0f));
    }

parent->trigger(this);
bHighlight=true;
}

void SliderButton::finishDrag(){

input->dragButton=NULL;
bHighlight=false;
}

void SliderButton::clickedLeft(){

if (bVertical)
  sliderValue=(input->mouseY-location.y)/scale.y;
else
  sliderValue=(input->mouseX-location.x)/scale.x;

parent->trigger(this);
}


void SliderButton::clickedRight(){}

void SliderButton::focusClick(){

BasicButton::focusClick();
}

void SliderButton::deselect(int depth){

BasicButton::deselect(depth);
}

void SliderButton::drawPlane(){

//hack for making it go all the way!
float addedScale=scale.y*0.1;
scale.y+=addedScale;

BasicButton::drawPlane();

scale.y-=addedScale;

    if (bHighlight)
        glColor4f(highlightColor.x,highlightColor.y,highlightColor.z, highlightColor.a);
    else
        glColor4f(slidePointColor.x,slidePointColor.y,slidePointColor.z, slidePointColor.a);

    bHighlight=false;

if (bVertical)
    {
     float slidePoint=sliderValue*scale.y;
     if (bFlipValue) slidePoint=scale.y - sliderValue*scale.y;

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,slidePoint,100.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0,slidePoint+scale.y*0.1,100.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(scale.x,slidePoint+scale.y*0.1,100.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(scale.x,slidePoint,100.0);
    glEnd();
    }

else
    {
     float slidePoint=sliderValue*scale.x - scale.x * sliderValue/10.0;
     if (bFlipValue) slidePoint=scale.x * sliderValue/10.0 - sliderValue*scale.x;

    glBegin(GL_QUADS);
		glTexCoord2f(0,0.0);
		glVertex3f(slidePoint,0.0,100.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(slidePoint+scale.x*0.1,0,100.0);
        glTexCoord2f(1.0,1.0);
		glVertex3f(slidePoint+scale.x*0.1,scale.y,100.0);
        glTexCoord2f(0.0,1.0);
		glVertex3f(slidePoint,scale.y,100.0);
	glEnd();
    }
}

void SliderButton::create(){sceneData->addButton(this);}
