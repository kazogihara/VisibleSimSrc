#include "flatTransform01Code.hpp"

FlatTransform01Code::FlatTransform01Code(Catoms3DBlock *host) : Catoms3DBlockCode(host), module(host)
{
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host)
        return;
    addMessageEventFunc2(PHASE01,
                         std::bind(&FlatTransform01Code::movePhase01, this,
                                   std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(PHASE04,
                         std::bind(&FlatTransform01Code::movePhase04, this,
                                   std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(PHASE06,
                         std::bind(&FlatTransform01Code::movePhase06, this,
                                   std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(PHASE08,
                         std::bind(&FlatTransform01Code::movePhase08, this,
                                   std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(PHASE09,
                         std::bind(&FlatTransform01Code::movePhase09, this,
                                   std::placeholders::_1, std::placeholders::_2));
}

void FlatTransform01Code::startup()
{
    console << "start " << module->blockId << "\n";
    if ((hasRightFrontTopNeighbor() and hasLeftBackBottomNeighbor()) or (hasLeftFrontTopNeighbor() and hasRightBackBottomNeighbor()))
    {
        module->setColor(RED);
        // Cell3DPosition targetPos = module->position + Cell3DPosition(1, 1, 1);
        // P2PNetworkInterface *itf = module->getInterface(targetPos);
        // sendMessage(new MessageOf<int>(PHASE01, 0),
        //             itf, 0, 0);
        // phase02_flag = true;
    }
}

void FlatTransform01Code::onMotionEnd()
{
    // complete with your code
    if (!moveStack.empty())
    {
        moveStack.front()();
        moveStack.pop();
    }
    else if (phase03_flag)
    {
        movePhase03();
    }
    else if (phase05_flag)
    {
        movePhase05();
    }
    else if (phase07_flag)
    {
        movePhase07();
    }
    console << " End of motion to " << module->position << "\n";
}

void FlatTransform01Code::onNeighborRemove()
{
    if (phase02_flag)
    {
        movePhase02();
        phase02_flag = false;
        phase03_flag = true;
    }
    else if (phase10_flag)
    {
        movePhase10();
        phase10_flag = false;
    }
    console << "Removed Neighbor " << module->position << "\n";
}

void FlatTransform01Code::movePhase01(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    moveStack.push([this]()
                   { this->moveLeftFrontBottom(); });
    moveStack.push([this]()
                   { this->moveRightBackBottom(); });

    moveRightBackTop();
};

void FlatTransform01Code::movePhase02()
{
    moveBack();
};

void FlatTransform01Code::movePhase03()
{
    Cell3DPosition targetPos = module->position + Cell3DPosition(0, 1, -1);
    P2PNetworkInterface *itf = module->getInterface(targetPos);
    sendMessage(new MessageOf<int>(PHASE04, 0),
                itf, 0, 0);
    phase03_flag = false;
};

void FlatTransform01Code::movePhase04(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    moveStack.push([this]()
                   { this->moveRightBackBottom(); });
    moveStack.push([this]()
                   { this->moveRight(); });
    moveLeftFrontTop();
    phase05_flag = true;
};

void FlatTransform01Code::movePhase05()
{
    Cell3DPosition targetPos = module->position + Cell3DPosition(-1, -1, 1);
    P2PNetworkInterface *itf = module->getInterface(targetPos);
    sendMessage(new MessageOf<int>(PHASE06, 0),
                itf, 0, 0);
};

void FlatTransform01Code::movePhase06(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    moveStack.push([this]()
                   { this->moveRightFrontTop(); });
    moveStack.push([this]()
                   { this->moveRightFrontTop(); });
    moveStack.push([this]()
                   { this->moveRight(); });
    moveStack.push([this]()
                   { this->moveRightBackBottom(); });
    moveFront();
    phase07_flag = true;
};

void FlatTransform01Code::movePhase07()
{
    Cell3DPosition targetPos = module->position + Cell3DPosition(-1, -1, -1);
    P2PNetworkInterface *itf = module->getInterface(targetPos);
    sendMessage(new MessageOf<int>(PHASE08, 0),
                itf, 0, 0);
};

void FlatTransform01Code::movePhase08(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    Cell3DPosition targetPos = module->position + Cell3DPosition(1, 0, -1);
    P2PNetworkInterface *itf = module->getInterface(targetPos);
    sendMessage(new MessageOf<int>(PHASE09, 0),
                itf, 0, 0);
    phase10_flag = true;
};

void FlatTransform01Code::movePhase09(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    moveStack.push([this]()
                   { this->moveRightFrontTop(); });
    moveRightBackBottom();
};

void FlatTransform01Code::movePhase10()
{
    moveStack.push([this]()
                   { this->moveRightFrontBottom(); });    
    moveRightBackTop();
};

bool FlatTransform01Code::hasFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(0, 1, 0)) != NULL;
}

bool FlatTransform01Code::hasBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(0, -1, 0)) != NULL;
}

bool FlatTransform01Code::hasRightNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, 0, 0)) != NULL;
}

bool FlatTransform01Code::hasLeftNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(-1, 0, 0)) != NULL;
}

bool FlatTransform01Code::hasLeftFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, -1, 0)) != NULL;
}

bool FlatTransform01Code::hasRightFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, 1, 0)) != NULL;
}

bool FlatTransform01Code::hasLeftBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(-1, -1, 0)) != NULL;
}

bool FlatTransform01Code::hasRightBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, -1, 0)) != NULL;
}

bool FlatTransform01Code::hasLeftFrontTopNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(-1, 0, 1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 1, 1)) != NULL;
    }
}

bool FlatTransform01Code::hasRightFrontTopNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 0, 1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(1, 1, 1)) != NULL;
    }
}

bool FlatTransform01Code::hasLeftBackTopNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(-1, -1, 1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 0, 1)) != NULL;
    }
}

bool FlatTransform01Code::hasRightBackTopNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, -1, 1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(1, 0, 1)) != NULL;
    }
}

bool FlatTransform01Code::hasLeftFrontBottomNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(-1, 0, -1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 1, -1)) != NULL;
    }
}

bool FlatTransform01Code::hasRightFrontBottomNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 0, -1)) != NULL;
    }
    else
    {

        return module->getNeighborOnCell(module->position + Cell3DPosition(1, 1, -1)) != NULL;
    }
}

bool FlatTransform01Code::hasLeftBackBottomNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(-1, -1, -1)) != NULL;
    }
    else
    {

        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 0, -1)) != NULL;
    }
}

bool FlatTransform01Code::hasRightBackBottomNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, -1, -1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(1, 0, -1)) != NULL;
    }
}

void FlatTransform01Code::moveLeft()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, 0, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRight()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(1, 0, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveTop()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 0, 1);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveBottom()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 0, -1);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(1, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftFrontTop()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(-1, 0, 1);
    }
    else
    {
        targetPos = module->position + Cell3DPosition(0, 0, 1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightFrontTop()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(0, 0, 1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(1, 1, 1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftBackTop()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(-1, -1, 1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(0, 0, 1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightBackTop()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(0, -1, 1);
    }
    else
    {
        targetPos = module->position + Cell3DPosition(1, 0, 1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftFrontBottom()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(-1, 0, -1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(0, 1, -1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightFrontBottom()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(0, 0, -1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(1, 1, -1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveLeftBackBottom()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(-1, -1, -1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(0, 0, -1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void FlatTransform01Code::moveRightBackBottom()
{
    Cell3DPosition targetPos;
    if (module->position.pt[2] % 2 == 0)
    {
        targetPos = module->position + Cell3DPosition(0, -1, -1);
    }
    else if (!(module->position.pt[2] % 2 == 0))
    {
        targetPos = module->position + Cell3DPosition(1, 0, -1);
    }
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}
