#ifndef H_INSPECTORMANAGER
#define H_INSPECTORMANAGER

#include "actor.h"
#include "inspector.h"

class InspectorManager: public Actor{

public:

    Inspector* lastOpenInspector;

    vector<Inspector*> inspectors;

    InspectorManager();
    virtual ~InspectorManager();

    virtual void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void inspectorOpened(Inspector* other);
    virtual void inspectorClosed(Inspector* other);

    virtual void closeAll();

    virtual void update(double deltaTime);

    virtual void create();
};
#endif // INSPECTORMANAGER
