//
// Created by al_sah on 04.04.21.
//

#include <fstream>
#include "PicturesTaker.h"


PicturesTaker::PicturesTaker(std::function<void(payload_type, void*, void*)> &callback, void* data) {
    this->module_id = "picturesTaker";
}

std::string PicturesTaker::getId() const {
    return this->module_id;
}

void PicturesTaker::executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {
    std::string file_string;
    if(payload == "camera"){
        system("resolution=`uvcdynctrl -f | grep \"Frame size:\" | sort|uniq | sed 's/.*: //' | sort -h | tail -n 1|head -n 1`\n"
               "fswebcam -r  $resolution --jpeg 100 -D 1 /tmp/cam-shot.jpg");

        std::ifstream file("/tmp/cam-shot.jpg");
        if (file.is_open()) {
            std::stringstream ss;
            ss << file.rdbuf();
            file_string = "cam-shot.jpg ";
            file_string += ss.str();
            file.close();
            callback(payload_type::binary_data, &file_string, true);
        }
    } else if(payload == "screenshot"){
        system("xfce4-screenshooter -f -s /tmp/screenshot.png");
        std::ifstream file("/tmp/screenshot.png");
        if (file.is_open()) {
            std::stringstream ss;
            ss << file.rdbuf();
            file_string = "screenshot.png ";
            file_string += ss.str();
            file.close();
            callback(payload_type::binary_data, &file_string, true);
        }
    }
}
