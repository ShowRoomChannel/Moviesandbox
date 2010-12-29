

#include "characterController.h"
#include "renderer.h"

CharacterController::CharacterController(){

controlledCharacter=NULL;

registerProperties();
}

CharacterController::~CharacterController(){}

void CharacterController::registerProperties(){

Actor::registerProperties();
}

void CharacterController::setup(){

Actor::setup();
}

void CharacterController::trigger(Actor * other){

        Actor::trigger(other);
}

void CharacterController::update(double deltaTime){

        Actor::update(deltaTime);
}

void CharacterController::create(){renderer->addActor(this);}