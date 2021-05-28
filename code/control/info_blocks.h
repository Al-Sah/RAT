#ifndef UI_INFO_BLOCKS_H
#define UI_INFO_BLOCKS_H

#include<QString>



struct WSRProperties{
    QString myID = "undefined";
    QString access_logging_level = "undefined";
    QString errors_logging_level = "undefined";
};

struct ConnectionMetaInfo{
    QString lastErrorCode = "undefined";
    QString lastErrorReason = "undefined";
    QString maxTransferringSize = "undefined";
    QString status = "undefined";
    QString status_details = "undefined";
    QString ID = "undefined";
    QString uri = "undefined";
    QString closeStatusCode = "undefined";
};






#endif // UI_INFO_BLOCKS_H

