//
// Created by al_sah on 04.06.21.
//
#include <cstdlib>

int main(){

    system("resolution=`uvcdynctrl -f | grep \"Frame size:\" | sort|uniq | sed 's/.*: //' | sort -h | tail -n 1|head -n 1`\n"
           "fswebcam -r  $resolution --jpeg 100 -D 1 /tmp/cam-shot.jpg");
    return 0;
}