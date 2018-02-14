#include "player.h"
#pragma comment(lib, "user32.lib")

void Player::EnableComputer(){
    computer = true;
}

void Player::DisableComputer(){
    computer = false;
}

bool Player::ComputerOn() const {
    return (computer==true);
}

bool Player::ComputerOff() const {
    return (computer==false);
}
