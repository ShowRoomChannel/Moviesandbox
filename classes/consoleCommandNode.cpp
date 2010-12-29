

#include "consoleCommandNode.h"
#include "control.h"
#include "renderer.h"
#include "input.h"

ConsoleCommandNode::ConsoleCommandNode(){

consoleCommand="NULL";

    listType.push_back("15TextInputButton");
    listName.push_back("command");
    listProp.push_back("CONSOLECOMMAND");


registerProperties();
}

ConsoleCommandNode::~ConsoleCommandNode(){}

void ConsoleCommandNode::registerProperties(){

createMemberID("CONSOLECOMMAND",&consoleCommand,this);
Node::registerProperties();
}

void ConsoleCommandNode::start(){

Node::start();
}

void ConsoleCommandNode::stop(){

Node::stop();
}

void ConsoleCommandNode::execute(){

    if (input->console->functionList[consoleCommand])
        input->console->callFunction(consoleCommand);
    else
        cout <<"Command not found!" << endl;

    nextNode();
}


void ConsoleCommandNode::update(double deltaTime){

	Node::update(deltaTime);
}

void ConsoleCommandNode::create(){renderer->addNode(this);}