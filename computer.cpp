#include "player.h"

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

void Player::PressAnyKey() const {
    Sleep(1000);
    keybd_event(VK_RETURN,0,0,0);
    keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
}
