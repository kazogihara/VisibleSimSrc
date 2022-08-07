#include "my3DCSnakeCode.hpp"

My3DCSnakeCode::My3DCSnakeCode(Catoms3DBlock *host) : Catoms3DBlockCode(host), module(host)
{
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host)
        return;
    addMessageEventFunc2(MSG_IS_RIGHT_END,
                         std::bind(&My3DCSnakeCode::isRightEnd, this,
                                   std::placeholders::_1, std::placeholders::_2));
    addMessageEventFunc2(MSG_IS_LEFT_END,
                         std::bind(&My3DCSnakeCode::isLeftEnd, this,
                                   std::placeholders::_1, std::placeholders::_2));
}

void My3DCSnakeCode::startup()
{
    if (module->blockId == 3)
    {
        console << "start " << module->blockId << "\n";
        console << "position " << module->position << "\n";
        if (hasRightNeighbor())
        { // If none, module->getInterface return Null
            targetPos = module->position + Cell3DPosition(1, 0, 0);
            P2PNetworkInterface *itf = module->getInterface(targetPos);
            sendMessage(new MessageOf<int>(MSG_IS_RIGHT_END, 0),
                        itf, 0, 0);
        }
        if (hasLeftNeighbor())
        {
            targetPos = module->position + Cell3DPosition(-1, 0, 0);
            P2PNetworkInterface *itf = module->getInterface(targetPos);
            sendMessage(new MessageOf<int>(MSG_IS_LEFT_END, 0),
                        itf, 0, 0);
        }
        else
        {
            createBranch();
        }
    }
}

void My3DCSnakeCode::parseUserBlockElements(TiXmlElement *config)
{
    const char *attr = config->Attribute("leader");
    if (attr != nullptr)
    {
        std::cout << module->blockId << " is leader!" << std::endl; // complete with your code
    }
}

void My3DCSnakeCode::onMotionEnd()
{
    createBranch();
}

void My3DCSnakeCode::createBranch()
{
    if (module->color == RED)
    {
        if (hasRightNeighbor())
        {
            moveRightFrontTop();
        }
        else if (hasRightBackBottomNeighbor())
        {
            moveRight();
        }
        else if(hasLeftNeighbor() or hasLeftBackBottomNeighbor())
        {
            sendIsLeftEnd();
        }
    }
    else
    {
        if (hasRightNeighbor())
        {
            moveRightBackTop();
        }
        else if (hasRightFrontBottomNeighbor())
        {
            moveRight();
        }
        else if(hasLeftNeighbor() or hasLeftFrontBottomNeighbor())
        {
            sendIsLeftEnd();
        }
    }
}

void My3DCSnakeCode::sendIsLeftEnd()
{
    if (module->color == RED)
    {
        if (module->position.pt[2] % 2 == 0)
        {
            targetPos = module->position + Cell3DPosition(-1, -1, -1);
        }
        else
        {
            targetPos = module->position + Cell3DPosition(0, 0, -1);
        }
    }
    else
    {
        if (module->position.pt[2] % 2 == 0)
        {
            targetPos = module->position + Cell3DPosition(-1, 0, -1);
        }
        else
        {
            targetPos = module->position + Cell3DPosition(0, 1, -1);
        }
    }
    P2PNetworkInterface *itf = module->getInterface(targetPos);
    sendMessage(new MessageOf<int>(MSG_IS_LEFT_END, 0),
                itf, 0, 0);
}

void My3DCSnakeCode::isRightEnd(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    if (hasRightNeighbor())
    {
        targetPos = module->position + Cell3DPosition(1, 0, 0);
        P2PNetworkInterface *itf = module->getInterface(targetPos);
        sendMessage(new MessageOf<int>(MSG_IS_RIGHT_END, 0),
                    itf, 0, 0);
    }
};

void My3DCSnakeCode::isLeftEnd(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{

    if (hasLeftBackBottomNeighbor())
    {
        if (module->position.pt[2] % 2 == 0)
        {
            targetPos = module->position + Cell3DPosition(-1, -1, -1);
        }
        else
        {
            targetPos = module->position + Cell3DPosition(0, 0, -1);
        }
        P2PNetworkInterface *itf = module->getInterface(targetPos);
        sendMessage(new MessageOf<int>(MSG_IS_LEFT_END, 0),
                    itf, 0, 0);
    }
    else if (hasLeftNeighbor())
    {
        targetPos = module->position + Cell3DPosition(-1, 0, 0);
        P2PNetworkInterface *itf = module->getInterface(targetPos);
        sendMessage(new MessageOf<int>(MSG_IS_LEFT_END, 0),
                    itf, 0, 0);
    }
    else
    {
        createBranch();
    }
}

bool My3DCSnakeCode::hasFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(0, 1, 0)) != NULL;
}

bool My3DCSnakeCode::hasBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(0, -1, 0)) != NULL;
}

bool My3DCSnakeCode::hasRightNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, 0, 0)) != NULL;
}

bool My3DCSnakeCode::hasLeftNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(-1, 0, 0)) != NULL;
}

bool My3DCSnakeCode::hasLeftFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, -1, 0)) != NULL;
}

bool My3DCSnakeCode::hasRightFrontNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, 1, 0)) != NULL;
}

bool My3DCSnakeCode::hasLeftBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(-1, -1, 0)) != NULL;
}

bool My3DCSnakeCode::hasRightBackNeighbor() const
{
    return module->getNeighborOnCell(module->position + Cell3DPosition(1, -1, 0)) != NULL;
}

bool My3DCSnakeCode::hasLeftFrontTopNeighbor() const
{
    if (module->position.pt[2] % 2 == 0)
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(-1, 0, 1)) != NULL;
    }
    else
    {
        return module->getNeighborOnCell(module->position + Cell3DPosition(0, 0, 1)) != NULL;
    }
}

bool My3DCSnakeCode::hasRightFrontTopNeighbor() const
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

bool My3DCSnakeCode::hasLeftBackTopNeighbor() const
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

bool My3DCSnakeCode::hasRightBackTopNeighbor() const
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

bool My3DCSnakeCode::hasLeftFrontBottomNeighbor() const
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

bool My3DCSnakeCode::hasRightFrontBottomNeighbor() const
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

bool My3DCSnakeCode::hasLeftBackBottomNeighbor() const
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

bool My3DCSnakeCode::hasRightBackBottomNeighbor() const
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

void My3DCSnakeCode::moveLeft()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, 0, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveRight()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(1, 0, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveTop()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 0, 1);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveBottom()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(0, 0, -1);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveLeftFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveRightFront()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(1, 1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveLeftBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveRightBack()
{
    Cell3DPosition targetPos;
    targetPos = module->position + Cell3DPosition(-1, -1, 0);
    scheduler->schedule(
        new Catoms3DRotationStartEvent(getScheduler()->now(), module, targetPos));
}

void My3DCSnakeCode::moveLeftFrontTop()
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

void My3DCSnakeCode::moveRightFrontTop()
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

void My3DCSnakeCode::moveLeftBackTop()
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

void My3DCSnakeCode::moveRightBackTop()
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

void My3DCSnakeCode::moveLeftFrontBottom()
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

void My3DCSnakeCode::moveRightFrontBottom()
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

void My3DCSnakeCode::moveLeftBackBottom()
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

void My3DCSnakeCode::moveRightBackBottom()
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