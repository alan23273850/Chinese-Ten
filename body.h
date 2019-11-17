#ifndef BODY_H
#define BODY_H

#include "card.h"

class Ranking;

void prepare_console_settings();
void print_welcome_screen();
void print_main_menu();
void check_the_ranking_table();
int choose_player_num();
int choose_computer_num(int player_num);
void input_names(int computer_num);
void print_player_order();
void shuffle_and_deal_cards();
void play_game_loop(Card::Rank special_rank);
void print_card_results();
Ranking print_scoreboard();
Card::Rank special_check();

#endif // BODY_H
