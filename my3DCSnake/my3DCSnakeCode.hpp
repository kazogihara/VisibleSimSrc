#ifndef my3DCSnakeCode_H_
#define my3DCSnakeCode_H_

#include "robots/catoms3D/catoms3DSimulator.h"
#include "robots/catoms3D/catoms3DWorld.h"
#include "robots/catoms3D/catoms3DBlockCode.h"
#include "robots/catoms3D/catoms3DMotionEngine.h"

using namespace Catoms3D;

static const uint MSG_IS_RIGHT_END = 1000;
static const uint MSG_IS_LEFT_END = 1001;
static const uint MSG_FIND_NEXT = 2000;
class My3DCSnakeCode : public Catoms3DBlockCode {
private:
	Catoms3DBlock *module = nullptr;
public :
  Catoms3D::Catoms3DBlock *catom;
	My3DCSnakeCode(Catoms3DBlock *host);
	~My3DCSnakeCode() {};
  Cell3DPosition targetPos;
/**
  * This function is called on startup of the blockCode, it can be used to perform initial
  *  configuration of the host or this instance of the program.
  * @note this can be thought of as the main function of the module
  */
    void startup() override;

/**
  * @brief Provides the user with a pointer to the configuration file parser, which can be used to read additional user information from each block config. Has to be overridden in the child class.
  * @param config : pointer to the TiXmlElement representing the block configuration file, all information related to concerned block have already been parsed
  *
  */
    void parseUserBlockElements(TiXmlElement *config) override;

/**
  * @brief Callback function executed whenever the module finishes a motion
  */
    void onMotionEnd() override;

    void isLeftEnd(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
    void isRightEnd(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
    void findNextMSG(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);    
    bool hasFrontNeighbor() const;
    bool hasBackNeighbor() const;
    bool hasRightNeighbor() const;
    bool hasLeftNeighbor() const;
    bool hasLeftFrontNeighbor() const;
    bool hasRightFrontNeighbor() const;
    bool hasLeftBackNeighbor() const;
    bool hasRightBackNeighbor() const;
    bool hasLeftFrontTopNeighbor() const;
    bool hasRightFrontTopNeighbor() const;
    bool hasLeftBackTopNeighbor() const;
    bool hasRightBackTopNeighbor() const;
    bool hasRightFrontBottomNeighbor() const;
    bool hasLeftFrontBottomNeighbor() const;
    bool hasRightBackBottomNeighbor() const;
    bool hasLeftBackBottomNeighbor() const;

    void moveLeft();
    void moveRight();
    void moveFront();
    void moveBack();
    void moveTop();
    void moveBottom();
    void moveLeftFront();
    void moveRightFront();
    void moveLeftBack();
    void moveRightBack();
    void moveLeftFrontTop();
    void moveRightFrontTop();
    void moveLeftBackTop();
    void moveRightBackTop();
    void moveLeftFrontBottom();
    void moveRightFrontBottom();
    void moveLeftBackBottom();
    void moveRightBackBottom();

    void sendIsLeftEnd();
    void createBranch();
/*****************************************************************************/
/** needed to associate code to module                                      **/
	static BlockCode *buildNewBlockCode(BuildingBlock *host) {
	    return(new My3DCSnakeCode((Catoms3DBlock*)host));
	}
/*****************************************************************************/
};

#endif /* my3DCSnakeCode_H_ */