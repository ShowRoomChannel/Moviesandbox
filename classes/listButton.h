#ifndef H_LISTBUTTON
#define H_LISTBUTTON

#include "basicButton.h"
#include "sliderButton.h"

class ListButton: public BasicButton{

public:

    int listDisplayMode;                            //mode 0: to the right and down
                                                    //mode 1: weird thing for brush-size
                                                    //mode 2: circular
                                                    //mode 3: straight up
                                                    //mode 4: box, right - used for texture browser
                                                    //mode 4: straight down

    float radius;                                   //for radial menu
    float maxRadius;                                //for radial menu
    float animSpeed;                                //for radial menu
    float listWidth;                                //width of every button in the list
    float listHeight;                               //height of every button in the list

    float listOffsetX;                              //used for non-standard-sized listButtons in placeButton
    float listOffsetY;                              //used for non-standard-sized listButtons in placeButton


    int listColumns;

    Vector3f listButtonSpacing;                    //spacing between listButtons
    Vector3f listLoc;                               //list position

    Vector3f listSize;                              //size of all buttons in screen-coordinates
    float scrollSize;                               //size of scrollbar
    float listDisplaySize;                          //how much of the list is to be displayed in screen-coordinates


    std::vector <std::string> listType;            //registers the types of Buttons you want the list to populate
    std::vector <std::string> listProp;            //the buttonProperty for each Button in the List
    std::vector <std::string> listName;            //the name for each Button in the List
    std::vector <std::string> listIcon;            //the icon for each Button in the List
    std::vector <std::string> listParent;          //the parent for each Button in the List (will call trigger on parent!)

    std::vector <BasicButton *> listButton;             //holds the actual references to the Buttons - will be populated by assembleList();

    SliderButton* scrollBar;                      //holds the scroll button;

    BasicButton* backgroundButton;                 //for list background/area

    bool bPermanentList;                           //make listButtons permanent = undeletable
                                                   //you must manually clean them up then!
    bool bRelativeListLocation;                     //list position is relative to listbutton
    bool bDrawListNames;                           //draws names for listbuttons
    bool bListOpen;                                //is the list currently on display or not?

    Vector4f listColor;                            //color of the buttons in the list
    string listShader;                             //shader to use (usually color or texture)


    /*** Functions ***/


    ListButton();
    virtual ~ListButton();

    virtual void setup();
    virtual void update(double deltaTime);

    virtual void clickedLeft();                    //calls assembleList() and placeButton()

    virtual void assembleList();                                    //creates Buttons from the listType vector and puts them in listButton vector as Button references
    virtual void placeButton(int buttonNumber, int drawPosition);   //sets location of listButtons
    virtual void createScrollBar();                                 //create slider to scroll through listButtons
    virtual void placeScrollBar();                                  //where it should appear

    virtual void trigger(MsbObject* other);                             //callback from Scrollbar

    virtual void deselect(int depth);


    virtual void create();
};
#endif
