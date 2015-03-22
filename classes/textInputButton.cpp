

#include "textInputButton.h"
#include "renderer.h"
#include "input.h"
#include "inputConnectButton.h"

TextInputButton::TextInputButton(){

name="TextInputButton";
parent=NULL;
buttonProperty="NULL";
bEditing=false;
bmIDPart=false;
mIDPartNumber=0;
}

TextInputButton::~TextInputButton(){}


void TextInputButton::setup(){
    BasicButton::setup();

    if (parent && buttonProperty!="NULL"){
        memberID * mID=&parent->property[buttonProperty];
        if (mID)
            tooltip=memberToString(mID,bmIDPart,mIDPartNumber);
        }
}

void TextInputButton::update(double deltaTime){

    BasicButton::update(deltaTime);

    if (bEditing){
        tooltip=input->inputText;
        if ((int)(renderer->currentTime)%1000 < 500)
            tooltip+="|";
    }else{

    if (parent && buttonProperty!="NULL"){
        memberID * mID=&parent->property[buttonProperty];
        if (mID)
            tooltip=memberToString(mID,bmIDPart,mIDPartNumber);
        }

    }
}

void TextInputButton::drawTooltip(){

    bOver=true;

    BasicButton::drawTooltip();
}

void TextInputButton::mouseOver(){

BasicButton::mouseOver();
}

void TextInputButton::mouseDrag(){}

void TextInputButton::clickedLeft(){

bEditing=true;
input->focusButton=this;
input->inputText="";
input->bTextInput=true;
//but also: read the type of the property, if we're connected to one and set it to the inputString!
if (parent && buttonProperty!="" && tooltip!=""){
    //go through tooltip and stop when we find a " "!
    char* myTooltip=(char*)tooltip.c_str();
    char* preText = strtok(myTooltip," "); // Splits spaces between words in st
    cout << "our pretext would be: " << preText << endl;
    input->inputText=preText;
    input->inputText+=' ';
    }
}

//use this to connect properties to InputConnect Button!
void TextInputButton::clickedRight(){

    input->focusButton=this;

    cout << "connecting..." << input->hudTarget->name << endl;

}

void TextInputButton::focusClick(){

    InputConnectButton* ipcb =NULL;
    ipcb=dynamic_cast<InputConnectButton*>(input->hudTarget);

    //connecting with InputConnectButton
    if (input->hudTarget && ipcb){

        cout << "connecting with: " << input->hudTarget << endl;
        input->hudTarget->buttonProperty=buttonProperty;                //which property do we want to connect?
        input->hudTarget->trigger(parent);                          //which actor do we want to connect
        cout << "connecting property: " << buttonProperty << endl;

    }else{

        std::stringstream ss;
        tooltip=(char*)input->inputText.c_str();

        //adding selectedActor stack to TextInputButton

        if (parent && buttonProperty!="NULL"){

            bool bParentSelected=false;
            for (int i=0;i<(int)sceneData->selectedActors.size();i++ )
                if (sceneData->selectedActors[i]==parent)
                    bParentSelected=true;

           if (bParentSelected){

                for (int i=0;i<(int)sceneData->selectedActors.size();i++){

                    memberID * mID = &(sceneData->selectedActors[i]->property[buttonProperty]);        //look for the property we should set
                    cout << "we found the following memberID: " << mID->memberName << " with the following member type: " << mID->memberType->name() << endl;
                    if (buttonProperty=="ROTATION"){
                        memberFromString(mID,input->inputText);
                        sceneData->selectedActors[i]->setRotation(sceneData->selectedActors[i]->rotation);
                    }else if (buttonProperty=="LOCATION"){
                        memberFromString(mID,input->inputText);
                        sceneData->selectedActors[i]->setLocation(sceneData->selectedActors[i]->location);
                    }else if (buttonProperty=="SCALE"){
                        memberFromString(mID,input->inputText);
                        sceneData->selectedActors[i]->setScale(sceneData->selectedActors[i]->scale);
                    }else if (mID){
                        memberFromString(mID,input->inputText);
                    }else
                        cout << "no such property: " << buttonProperty;
                }
           }
           else{
               cout << "we are connected to " << parent->name <<endl;

               memberID * mID = &(parent->property[buttonProperty]);        //look for the property we should set
               cout << "we found the following memberID: " << mID->memberName << " with the following member type: " << mID->memberType->name() << endl;
               if (mID)
                 {
                 memberFromString(mID,input->inputText);
                 }
               else
                 cout << "no such property: " << buttonProperty;
               }
          }

        if (parent)
          parent->trigger(this);

        //clean up

        input=Input::getInstance();

        input->inputText="NULL";

        bEditing=false;
        }
    BasicButton::focusClick();
}

void TextInputButton::deselect(int depth){

BasicButton::deselect(depth);
}

void TextInputButton::create(){sceneData->addButton(this);}
