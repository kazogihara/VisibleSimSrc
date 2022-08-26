#ifndef flatTransform01Code_H_
#define flatTransform01Code_H_

#include "robots/catoms3D/catoms3DSimulator.h"
#include "robots/catoms3D/catoms3DWorld.h"
#include "robots/catoms3D/catoms3DBlockCode.h"
#include <queue>

using namespace Catoms3D;

class FlatTransform01Code : public Catoms3DBlockCode {
private:
	Catoms3DBlock *module = nullptr;
  std::queue<function<void(void)>> moveStack;
  bool phase02_flag = false;
  bool phase03_flag = false;
  bool phase05_flag = false;  
  bool phase06_flag = false;
  bool phase07_flag = false;  
  bool phase08_flag = false;
  bool phase09_flag = false;
  bool phase10_flag = false;
public :
	FlatTransform01Code(Catoms3DBlock *host);
	~FlatTransform01Code() {};
  static const uint PHASE01 = 1001;
  static const uint PHASE02 = 1002;
  static const uint PHASE03 = 1003;
  static const uint PHASE04 = 1004;
  static const uint PHASE05 = 1005;
  static const uint PHASE06 = 1006;
  static const uint PHASE07 = 1007;
  static const uint PHASE08 = 1008;
  static const uint PHASE09 = 1009;

  void movePhase01(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
  void movePhase02();
  void movePhase03();
  void movePhase04(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
  void movePhase05();
  void movePhase06(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
  void movePhase07();
  void movePhase08(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);
  void movePhase09(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender);  
  void movePhase10();  
/**
  * This function is called on startup of the blockCode, it can be used to perform initial
  *  configuration of the host or this instance of the program.
  * @note this can be thought of as the main function of the module
  */
    void startup() override;

/**
  * @brief Provides the user with a pointer to the configuration file parser, which can be used to read additional user information from it.
  * @param config : pointer to the TiXmlDocument representing the configuration file, all information related to VisibleSim's core have already been parsed
  *
  * Called from BuildingBlock constructor, only once.
  */
    void parseUserElements(TiXmlDocument *config) override {};

/**
  * @brief Callback function executed whenever the module finishes a motion
  */
    void onMotionEnd() override;

    void onNeighborRemove() override;

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

/*****************************************************************************/
/** needed to associate code to module                                      **/
	static BlockCode *buildNewBlockCode(BuildingBlock *host) {
	    return(new FlatTransform01Code((Catoms3DBlock*)host));
	}
/*****************************************************************************/
};

#endif /* flatTransform01Code_H_ */