//
// Created by al_sah on 16.07.21.
//

#ifndef ABSTRACT_CORE_WEB_SOCKET_MESSAGE_SENDER
#define ABSTRACT_CORE_WEB_SOCKET_MESSAGE_SENDER

/** @interface WebSocketMessageSender.
 *
 * WebSocketMessageSender is an interface which allows sending messages and
 * checking maximum message size which can b transferred
 *
 * This interface is implemented by @class AbstractWebsocketRunner,
 * used in @class AbstractTaskManager
 **/

#include "WebSocketResources.h"

class WebSocketMessageSender {
public:
    virtual bool send(const std::string &message, PayloadType pt)=0;
    virtual Size getMaxTransferringSize()=0;
};

#endif //ABSTRACT_CORE_WEB_SOCKET_MESSAGE_SENDER