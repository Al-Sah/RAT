//
// Created by al_sah on 17.07.21.
//

#include "../TaskManager.h"

inline std::string cmBool2str(bool b){
    return b ? "1" : "0";
}


void TaskManager::handleTaskResult(TaskResult &message, ParsedTextMessage &parsedTextMessage) {

    // TODO get max transferring message size, check it parse message by parts

    size_t max_message_size = messagesSender->getMaxTransferringSize(), current_message_size = message.getPayload().size();
    size_t parts, message_part_size;


    if(current_message_size + 150 > max_message_size){
        long double temp = (long double)current_message_size / max_message_size;
        parts = temp - (int)temp > 0 ? (int)(temp + 1) : (int)temp;
        // add max headers size to result message

        message_part_size = current_message_size / parts; //

        temp = ((long double)current_message_size + (long double)parts*150) / max_message_size;
        parts = temp - (int)temp > 0 ? (int)(temp + 1) : (int)temp;

        message_part_size = (current_message_size - parts*150) / parts; //

        const std::string& taskId = message.getTaskId();

        std::list<std::string> result_message_parts;

        std::string payload_to_parse;

        if(message.getPayloadType() == PayloadType::filePath){
            std::ifstream file(message.getPayload());
            if (file.is_open()) {
                std::stringstream ss;
                ss << file.rdbuf();
                payload_to_parse = ss.str();
                file.close();
            } else{
                std::cout << "Failed to read file!";
            }
        } else {
            payload_to_parse = message.getPayload();
        }

        for(int i = 1; i <= parts; i++){
            std::string result;
            if( i == 1){
                result = generate_section(properties.keys.packageType, properties.packages.firstPart)
                        .append(generate_section(properties.keys.requestId, message.getTaskId()))
                        .append(generate_section(properties.keys.isLast, cmBool2str(message.getIsLast())))
                        .append(generate_section(properties.keys.fullPayloadSize, std::to_string(message.getPayload().size())));

                TaskManager::length_check(result);
                result += payload_to_parse.substr(0,message_part_size);
                std::cout << " Sending bytes from    0  to  " << message_part_size-1 << std::endl;
            } else if (i == parts){
                result = generate_section(properties.keys.packageType, properties.packages.lastPart)
                        .append(generate_section(properties.keys.requestId, message.getTaskId()))
                        .append(generate_section(properties.keys.isLast, cmBool2str(message.getIsLast())));
                TaskManager::length_check(result);
                result += payload_to_parse.substr(message_part_size*(i-1));
                std::cout << " Sending bytes from    0  to  " << message_part_size*(i-1)  << "  " << payload_to_parse.substr(message_part_size*(i-1)).size() << std::endl;
            } else{
                result = generate_section(properties.keys.packageType, properties.packages.continuation)
                        .append(generate_section(properties.keys.requestId, message.getTaskId()));

                TaskManager::length_check(result);
                result += payload_to_parse.substr(message_part_size*(i-1), message_part_size);
                std::cout << " AAAAA " << message_part_size*(i-1) << "    "   << message_part_size*i-1  << std::endl;
            }
            result_message_parts.push_back(result);
        }
        for (const auto& part: result_message_parts){
            std::cout << " \n  SENDING " <<  part.size();
            this->messagesSender->send(part, message.getPayloadType());
        }

    } else{
        std::string result;

        result = generate_section(properties.keys.packageType, properties.packages.singleMessage) +
                 generate_section(properties.keys.requestId, message.getTaskId()) +
                 generate_section(properties.keys.isLast, cmBool2str(message.getIsLast()));

        TaskManager::length_check(result);

        if(message.getPayloadType() == PayloadType::text){
            result.append(message.getPayload());
            this->messagesSender->send(result, PayloadType::text);
        } else if (message.getPayloadType() == PayloadType::filePath){
            std::ifstream file(message.getPayload());
            if (file.is_open()) {
                std::string file_string;
                std::stringstream ss;
                ss << file.rdbuf();
                file_string = ss.str();
                file.close();
                result.append(file_string);

                this->messagesSender->send(result, PayloadType::binaryData);
            } else{
                std::cout << "Failed to read file!";
            }
        } else{
            result.append(message.getPayload());
            this->messagesSender->send(result, PayloadType::binaryData);
        }
    }
}

TaskManager::TaskManager(std::string moduleId, std::string version, cm::CommandsManagerProperties &properties)
        : AbstractTaskManager(moduleId, version, properties) {}
