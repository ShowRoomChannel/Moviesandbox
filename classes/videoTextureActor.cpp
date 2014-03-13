

#include "videoTextureActor.h"
#include "renderer.h"

VideoTextureActor::VideoTextureActor(){

    videoInfo="video.xml";
    bPlaying = false;
    playSpeed=1.0f;
    sceneShaderID="texture";
    setTextureID("videoTexture");
    drawType = DRAW_PLANE;
    bTextured= true;
    bReload= false;
    registerProperties();

}

VideoTextureActor::~VideoTextureActor(){

    player->closeMovie();
}

void VideoTextureActor::registerProperties(){

    createMemberID("PLAYSPEED",&playSpeed,this);
    createMemberID("VIDEOINFO",&videoInfo,this);
    createMemberID("RELOAD",&bReload,this);
    Actor::registerProperties();

}

void VideoTextureActor::setup(){

player=new ofVideoPlayer;
loadMovie("resources/"+videoInfo);
string myVidTexName=videoInfo;
myVidTexName.resize(myVidTexName.size()-4);
setTextureID(myVidTexName);
sceneData->textureList[myVidTexName]=new textureObject;
cout << "Setting Texture ID of VideoTexture Actor to: "<< myVidTexName << endl;
cout << "Is: "<< textureID << endl;
player->setSpeed(playSpeed);     //44 to 48 kHz problem...
Actor::setup();
player->setLoopState(OF_LOOP_NONE);
player->play();
}

void VideoTextureActor::postLoad(){

    Actor::postLoad();
    setup();

}

void VideoTextureActor::trigger(MsbObject* other){

    cout << "triggered video!" << endl;
/*
    if(bPlaying ) player->stop();
    else          player->play();

    bPlaying = !bPlaying;
    */
}

void VideoTextureActor::update(double deltaTime){

    Actor::update(deltaTime);

    player->play();

/*
    if (bReload==true){
        bReload=false;
        player->close();
        setup();
    }
*/
    if (!player){
        cout << "missing player object!" << endl;
        return;
    }
    // idle movie
    player->idleMovie();

    // get texture
    GLuint texture = sceneData->textureList[textureID]->texture;

    // select our texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );

    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, player->width, player->height, 0, GL_RGB, GL_UNSIGNED_BYTE, player->getPixels() );

}

void VideoTextureActor::stop(){

    Actor::stop();
    player->firstFrame();
    player->stop();
    bPlaying=false;
}

void VideoTextureActor::create(){sceneData->addActor(this);}



void VideoTextureActor::loadMovie(string fileName){


    TiXmlDocument doc( fileName );
    if (!doc.LoadFile()) return;


    TiXmlHandle hDoc(&doc);
    TiXmlElement * element;
    TiXmlHandle hRoot(0);

    element=hDoc.FirstChildElement().Element();
    // should always have a valid root but handle gracefully if it doesn't
    if (!element) return;
    // save this for later
    hRoot=TiXmlHandle(element);

    element=hRoot.FirstChild( "Video" ).Element();
    string vFileName=element->Attribute("vidFilename");

    cout << "LOADING video file: " << vFileName << endl;
    player->loadMovie(vFileName);
    player->setLoopState(OF_LOOP_NONE);
}
