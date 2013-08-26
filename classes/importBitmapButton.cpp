

#include "importBitmapButton.h"
#include "renderer.h"
#include "input.h"
#include "drawingWidget.h"
#include "meshData.h"
#include "drawTool.h"

ImportBitmapButton::ImportBitmapButton(){

buttonProperty="NULL";
color=Vector4f(0.5,0.5,0.5,1.0);

listWidth=200;
listColor=Vector4f(0.4,0.4,0.4,1.0);
listShader="buttonColor";
}

ImportBitmapButton::~ImportBitmapButton(){}

void ImportBitmapButton::mouseDrag(){}

void ImportBitmapButton::clickedLeft(){

    //LoadButton::clickedLeft();

    //remember our current Tool
    int oldTool=sceneData->controller->tool;

    if (!sceneData->brush->drawing){

        //TODO: make this optional
        sceneData->drawTool->createNewDrawing(true);

        //sceneData->makeWarningPopUp("No drawing! \n please place a drawing first!", this);
        //return;
    }

    string fileName=sceneData->openFileDialog("tga");
    loadFile(fileName);

    //switch back to it
    sceneData->controller->switchTool(oldTool);
}

void ImportBitmapButton::trigger(MsbObject*other){

}

void ImportBitmapButton::assembleLoadList(){

    parent=NULL;
    for (unsigned int i=0;i<sceneData->userImages.size();i++)
      {
        listType.push_back("12AssignButton");
        listName.push_back(sceneData->userImages[i]);
        listProp.push_back("NULL");
        listIcon.push_back("icon_flat");
      }
}

void ImportBitmapButton::placeScrollBar(){

    //scrollBar->setLocation(Vector3f(location.x + listWidth, location.y- scale.y -, 0 ) );
    //scrollBar->bFlipValue=true;

}

void ImportBitmapButton::loadFile(string filename){


    cout << "loading image " << filename << endl;
    //string fullName="resources/images/";
    //fullName.append(filename);
    FIBITMAP * myBitmap = FreeImage_Load(FIF_TARGA,filename.c_str(),0);
    if (!myBitmap){
        cout << "Error loading bitmap!" << endl;
        return;
    }


    int imageWidth=FreeImage_GetWidth(myBitmap);
    int imageHeight=FreeImage_GetHeight(myBitmap);
    //never used:
    //int channels=FreeImage_GetBPP(myBitmap)/8;
    if (sceneData->controller->tool!=TOOL_DRAW)
        sceneData->controller->switchTool(TOOL_DRAW);
    //aquire brush if existing
    assembleImage(myBitmap,imageWidth, imageHeight, 1.0f);
    FreeImage_Unload(myBitmap);

    //this is for backside loading!!!
        if (1){
            //fullName="resources/images/";

            int pos = filename.find(".tga");    // position of ".tga" in str
            string str = filename.substr(0,pos);   // get until ".tga" to the end
            str.append("_back.tga");
            //fullName.append(str);
            //fullName.append("asian_back.tga");
            cout << "filename to load is: " << str << endl;

            myBitmap = FreeImage_Load(FIF_TARGA,str.c_str(),0);
            if (!myBitmap){
                cout << "Error loading bitmap!" << endl;
                return;
            }

            imageWidth=FreeImage_GetWidth(myBitmap);
            imageHeight=FreeImage_GetHeight(myBitmap);

            assembleImage(myBitmap,imageWidth, imageHeight, -1.0f);
            FreeImage_Unload(myBitmap);
        }

}


void ImportBitmapButton::assembleImage(FIBITMAP* myBitmap, int imageWidth, int imageHeight, float flip){




    sceneData->brush->bMouseControlled=false;
    //go through all of the image
    for (int h=0;h<imageHeight;h++){
    //reset after every line
        for (int w=0;w<imageWidth;w++){

            RGBQUAD *myColor=new RGBQUAD;

            FreeImage_GetPixelColor(myBitmap,w,h,myColor);
            sceneData->brush->color.r=(float)myColor->rgbRed/255.0f;
            sceneData->brush->color.g=(float)myColor->rgbGreen/255.0f;
            sceneData->brush->color.b=(float)myColor->rgbBlue/255.0f;

            Vector3f bLoc;
            bLoc.x=sceneData->brush->drawing->location.x- 10 * (float) ((float)imageWidth/2.0 - (float)w)/(float)imageWidth;
            bLoc.y=sceneData->brush->drawing->location.y+ 10 *  (float)h/(float)imageHeight;
            bLoc.z=sceneData->brush->drawing->location.z- flip * (float)myColor->rgbReserved/32.0f;
            sceneData->brush->setLocation(bLoc);
            //flip y and z when shift down
            if (input->bShiftDown){
                bLoc.z=sceneData->brush->drawing->location.z+ 10 *  (float)h/(float)imageHeight;
                bLoc.y=sceneData->brush->drawing->location.y+ flip * (float)myColor->rgbReserved/32.0f;
                sceneData->brush->setLocation(bLoc);
            }

            Vector3f xyzNormal;

            if ((int)myColor->rgbReserved>0){

                input->mouseVector=Vector3f(0.1,0.1,0.1);   //HACK so that we actually paint something!
                sceneData->brush->setLocation(sceneData->brush->location*sceneData->brush->drawing->scale);
				input->mouse3D=sceneData->brush->location;
                //xyzNormal=genNormal(myBitmap,w,h,imageWidth,imageHeight,flip);
                //xyzNormal+=genNormal(myBitmap,w+1,h,imageWidth,imageHeight,flip);
                //xyzNormal+=genNormal(myBitmap,w-1,h,imageWidth,imageHeight,flip);
                //xyzNormal+=genNormal(myBitmap,w,h+1,imageWidth,imageHeight,flip);
                //xyzNormal+=genNormal(myBitmap,w,h-1,imageWidth,imageHeight,flip);
                //xyzNormal.normalize();
                //sceneData->brush->pNormal=xyzNormal;

                ((DrawTool*)sceneData->controller->currentTool)->paint();

            ///fill holes

                float myDepth=(float)myColor->rgbReserved;
                float fillStep=1.0;

                //find lowest fillDepth around us
                FreeImage_GetPixelColor(myBitmap,w-1,h,myColor);
                float fillDepth =(float)myColor->rgbReserved;

                FreeImage_GetPixelColor(myBitmap,w+1,h,myColor);
                float fillDepthRight =(float)myColor->rgbReserved;
                if (fillDepthRight<fillDepth)
                    fillDepth=fillDepthRight;

                FreeImage_GetPixelColor(myBitmap,w,h-1,myColor);
                float fillDepthDown =(float)myColor->rgbReserved;
                if (fillDepthDown<fillDepth)
                    fillDepth=fillDepthDown;

                FreeImage_GetPixelColor(myBitmap,w,h+1,myColor);
                float fillDepthUp =(float)myColor->rgbReserved;
                if (fillDepthUp<fillDepth)
                    fillDepth=fillDepthUp;

                if (fillDepth + fillStep/32.0f <myDepth){
                    int i=1;
                     if (!sceneData->brush->drawing){
                        sceneData->makeWarningPopUp("No drawing! \n please place a drawing first!", this);
                        return;
                    }
                while   (fillDepth<myDepth){
                        sceneData->brush->setLocation(sceneData->brush->location+ Vector3f(0,0,fillStep/32.0f));
                        input->mouse3D=sceneData->brush->location;
						 ((DrawTool*)sceneData->controller->currentTool)->paint();
                        fillDepth+=fillStep;
                        i++;
                    }
                }

        ///end fill holes

                //flip y and z when shift down
                //if (input->bShiftDown){
                    //float interim=xyzNormal.y;
                    //xyzNormal.y=xyzNormal.z;
                    //xyzNormal.z=interim;
                //}

            //sceneData->brush->pNormal=xyzNormal;

            }//filling holes
        }//inner for loop
    }//outer for loop

    sceneData->brush->bMouseControlled=true;

    //convert to triangles

    sceneData->vboList["testTriangle"]=new MeshData;
    sceneData->vboList["testTriangle"]->meshType=MESH_VBO;
    sceneData->vboList["testTriangle"]->bIsSkeletal=false;
    sceneData->vboList["testTriangle"]->bVertexColor=false;
    sceneData->vboList["testTriangle"]->boneCount=0;
    int vID=0;

    for (int h=0;h<imageHeight;h++){
    //reset after every line
        for (int w=0;w<imageWidth;w++){

            //get color
            RGBQUAD *myColor=new RGBQUAD;
            RGBQUAD *myColorTwo=new RGBQUAD;
            RGBQUAD *myColorThree=new RGBQUAD;
            RGBQUAD *myColorFour=new RGBQUAD;

            FreeImage_GetPixelColor(myBitmap,w,h,myColor);
            FreeImage_GetPixelColor(myBitmap,w+1,h,myColorTwo);
            FreeImage_GetPixelColor(myBitmap,w+1,h+1,myColorThree);
            FreeImage_GetPixelColor(myBitmap,w,h+1,myColorFour);

            if ((float)myColor->rgbReserved>0.0f && w<imageWidth-2 && h<imageHeight-2){

                vertexData myVData;

                //vertexOne
                Vector3f bLoc;
                bLoc.x=10.0 * (float) ((float)imageWidth/2.0 - (float)w)/(float)imageWidth;
                bLoc.y=10.0 *(float)h/(float)imageHeight;
                bLoc.z=(float)myColor->rgbReserved/32.0f;
                //first vertex
                myVData.location=Vector4f(bLoc.x,bLoc.y,bLoc.z,1.0);
                //myVData.location=Vector4f(0,1,0,0.0);
                myVData.vertexID=vID;
                vID++;

                sceneData->vboList["testTriangle"]->vData.push_back(myVData);


                //vertexTwo
                bLoc.x=10.0 * (float) ((float)imageWidth/2.0 - (float)w+1)/(float)imageWidth;
                bLoc.y=10.0 *(float)h/(float)imageHeight;
                bLoc.z=(float)myColorTwo->rgbReserved/32.0f;
                //first vertex
                myVData.location=Vector4f(bLoc.x,bLoc.y,bLoc.z,1.0);
                myVData.vertexID=vID;
                vID++;

                sceneData->vboList["testTriangle"]->vData.push_back(myVData);

                //vertexThree
                bLoc.x=10.0 * (float) ((float)imageWidth/2.0 - (float)w)/(float)imageWidth;
                bLoc.y=10.0 *(float)(h+1.0)/(float)imageHeight;
                bLoc.z=(float)myColorThree->rgbReserved/32.0f;
                //first vertex
                myVData.location=Vector4f(bLoc.x,bLoc.y,bLoc.z,1.0);
                myVData.vertexID=vID;
                vID++;

                sceneData->vboList["testTriangle"]->vData.push_back(myVData);

                //if (vID>300)
                    //return;
            }                //look for next vertex
        }
    }
}

Vector3f ImportBitmapButton::genNormal(FIBITMAP* myBitmap, int w, int h, int width, int height, float flip){

        //remember folks - the image is flipped!

        Vector4f influences=Vector4f(0,0,0,0);

        RGBQUAD *myColor=new RGBQUAD;

        Vector3f myNormal;

        FreeImage_GetPixelColor(myBitmap,w,h,myColor);
        float myInfluence=(float)myColor->rgbReserved;
        if (myInfluence==0){
            //cout << "bad Normal!!!" << endl;
            return Vector3f(0.0,0.0,0.0);
        }
          //Gathering influences
        if (w>=2 && w<width-2 && h>=2 && h<height-2){

            FreeImage_GetPixelColor(myBitmap,w-1,h,myColor);
            influences.x=(int)myColor->rgbReserved;

            FreeImage_GetPixelColor(myBitmap,w+1,h,myColor);
            influences.z=(int)myColor->rgbReserved;


            FreeImage_GetPixelColor(myBitmap,w,h+1,myColor);
            influences.y=(int)myColor->rgbReserved;


            FreeImage_GetPixelColor(myBitmap,w,h-1,myColor);
            influences.w=(int)myColor->rgbReserved;
        }


        float xN,yN,zN, falloffN;

        falloffN= 16.0;
        //edges
        if (influences.x==0){
            myNormal.x+=-1.0;
            myNormal.z=0.0;
        }

        if (influences.z==0){
            myNormal.x+=1.0;
            myNormal.z=0.0;
        }

        if (influences.y==0){
            myNormal.y+=1.0;
            myNormal.z=0.0;
        }

        if (influences.w==0){
            myNormal.y+=-1.0;
            myNormal.z=0.0;
        }

        if (myNormal!=Vector3f(0,0,0)){
            return myNormal;
        }

        //make linear, although we know it's wrong... it looks good :-)
        xN = (influences.x-influences.z)/falloffN;
        yN = (influences.w-influences.y)/falloffN;
        zN = -max(0.0,1.0-xN-yN);


        myNormal=Vector3f(xN,yN,zN * flip);

        myNormal.normalize();
        return myNormal;
}

void ImportBitmapButton::create(){sceneData->addButton(this);}
