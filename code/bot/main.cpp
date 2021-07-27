#include "Bot.h"

int main() {
    Bot *bot = Bot::GetInstance();
    bot->runPerpetual();
    return 0;
}