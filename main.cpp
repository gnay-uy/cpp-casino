#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <algorithm>
#include <thread>

//-----[[ COLOUR CONTROL ]]-----//
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define CLEAR "\033[2J\033[1;1H"

//-----[[ SYMBOLS ]]-----//
#define SPADE "\u2660"
#define HEART "\u2665"
#define DIAMOND "\u2666"
#define CLUB "\u2663"

#define CHERRY "\U0001F352"
#define LEMON "\U0001F34B"
#define ORANGE "\U0001F34A"
#define BELL "\U0001F514"
#define STAR "\u2B50"
#define SEVEN "7"
#define BAR "▃"

//#define CHERRY "🍒"
//#define LEMON "🍋"
//#define ORANGE "🍊"
//#define BELL "🔔"
//#define STAR "⭐"
//#define SEVEN "7"
//#define BAR "▃"

namespace games {
    namespace slots {
        const char *slot_template = R"(
┌─────┬─────┬─────┐
│ %s  │ %s  │ %s  │
└─────┴─────┴─────┘
)";

        void print_reels(const std::string &reel1, const std::string &reel2, const std::string &reel3) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), slot_template, reel1.c_str(), reel2.c_str(), reel3.c_str());
            std::cout << buffer;
        }

        std::vector<std::vector<std::string>> make_reels() {
            std::random_device rd;
            std::mt19937 g(rd());

            std::vector<std::vector<std::string>> reels = {
                    {CHERRY, LEMON, BELL, ORANGE, STAR, SEVEN, BAR},
                    {CHERRY, LEMON, BELL, ORANGE, STAR, SEVEN, BAR},
                    {CHERRY, LEMON, BELL, ORANGE, STAR, SEVEN, BAR}
            };

            for (auto &reel : reels) {
                std::shuffle(reel.begin(), reel.end(), g);
            }

            return reels;
        }

        bool game_main() {
            std::vector<std::vector<std::string>> reels = make_reels();

            for (size_t i = 0; i < reels[0].size(); ++i) {
                print_reels(reels[0][i], reels[1][i], reels[2][i]);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            return false;
        }
    }
    namespace blackjack {
        const char *card_template = R"(
┌────────┐
│%s      │
│        │
│   %s    │
│        │
│      %s│
└───%s───┘
)";

        const char *hidden_card_template = R"(
┌────────┐
│░░░░░░░░│
│░░░░░░░░│
│░░░░░░░░│
│░░░░░░░░│
│░░░░░░░░│
└────────┘
)";

        struct Card {
            int value;
            std::string suit;
            std::string rank;
        };

        const std::vector<Card> valid_cards = {
                {2, SPADE, "2"},
                {3, SPADE, "3"},
                {4, SPADE, "4"},
                {5, SPADE, "5"},
                {6, SPADE, "6"},
                {7, SPADE, "7"},
                {8, SPADE, "8"},
                {9, SPADE, "9"},
                {10, SPADE, "10"},
                {10, SPADE, "J"},
                {10, SPADE, "Q"},
                {10, SPADE, "K"},
                {2, HEART, "2"},
                {3, HEART, "3"},
                {4, HEART, "4"},
                {5, HEART, "5"},
                {6, HEART, "6"},
                {7, HEART, "7"},
                {8, HEART, "8"},
                {9, HEART, "9"},
                {10, HEART, "10"},
                {10, HEART, "J"},
                {10, HEART, "Q"},
                {10, HEART, "K"},
                {2, DIAMOND, "2"},
                {3, DIAMOND, "3"},
                {4, DIAMOND, "4"},
                {5, DIAMOND, "5"},
                {6, DIAMOND, "6"},
                {7, DIAMOND, "7"},
                {8, DIAMOND, "8"},
                {9, DIAMOND, "9"},
                {10, DIAMOND, "10"},
                {10, DIAMOND, "J"},
                {10, DIAMOND, "Q"},
                {10, DIAMOND, "K"},
                {2, CLUB, "2"},
                {3, CLUB, "3"},
                {4, CLUB, "4"},
                {5, CLUB, "5"},
                {6, CLUB, "6"},
                {7, CLUB, "7"},
                {8, CLUB, "8"},
                {9, CLUB, "9"},
                {10, CLUB, "10"},
                {10, CLUB, "J"},
                {10, CLUB, "Q"},
                {10, CLUB, "K"},
                {11, SPADE, "A"},
                {11, HEART, "A"},
                {11, DIAMOND, "A"},
                {11, CLUB, "A"}
        };

        std::vector<Card> deck;

        void shuffle_deck() {
            std::random_device rd;
            std::mt19937 g(rd());

            deck = valid_cards;
            std::shuffle(deck.begin(), deck.end(), g);
        }

        Card draw_card() {
            Card card = deck.back();
            deck.pop_back();
            return card;
        }

        int calculate_score(std::vector<Card> &hand) {
            int score = 0;
            int ace_count = 0;
            for (auto &card : hand) {
                score += card.value;
                if (card.rank == "A") ace_count++;
            }
            while (score > 21 && ace_count > 0) {
                score -= 10;
                ace_count--;
            }
            return score;
        }

        std::string format_value(int value) {
            if (value < 10) {
                return std::to_string(value) + "─";
            }
            return std::to_string(value);
        }

        std::vector<std::string> create_card(const Card& card) {
            std::array<char, 256> card_buffer;
            std::string val = format_value(card.value);
            std::string top_rank = card.rank;
            std::string bottom_rank = card.rank;

            if (card.rank.length() == 1) {
                top_rank += " ";
                bottom_rank = " " + bottom_rank;
            }

            std::snprintf(
                    card_buffer.data(), card_buffer.size(),
                    card_template,
                    top_rank.c_str(), card.suit.c_str(), bottom_rank.c_str(), val.c_str());

            std::vector<std::string> card_lines;
            std::string card_str(card_buffer.data());
            size_t pos = 0;
            std::string line;
            while ((pos = card_str.find('\n')) != std::string::npos) {
                line = card_str.substr(0, pos);
                if (!line.empty()) {
                    card_lines.push_back(line);
                }
                card_str.erase(0, pos + 1);
            }
            if (!card_str.empty()) {
                card_lines.push_back(card_str);
            }
            return card_lines;
        }

        std::vector<std::string> create_hidden_card() {
            std::vector<std::string> hidden_card_lines;
            std::string hidden_card_str(hidden_card_template);
            size_t pos = 0;
            std::string line;
            while ((pos = hidden_card_str.find('\n')) != std::string::npos) {
                line = hidden_card_str.substr(0, pos);
                if (!line.empty()) {
                    hidden_card_lines.push_back(line);
                }
                hidden_card_str.erase(0, pos + 1);
            }
            if (!hidden_card_str.empty()) {
                hidden_card_lines.push_back(hidden_card_str);
            }
            return hidden_card_lines;
        }

        void render_cards(const std::vector<Card>& cards, bool first_round = false) {
            if (cards.empty()) return;

            std::vector<std::vector<std::string>> all_card_lines;

            for (size_t i = 0; i < cards.size(); ++i) {
                if (first_round && i == 1) {
                    all_card_lines.push_back(create_hidden_card());
                } else {
                    all_card_lines.push_back(create_card(cards[i]));
                }
            }

            for (size_t line = 0; line < all_card_lines[0].size(); ++line) {
                for (const auto& card_lines : all_card_lines) {
                    std::cout << card_lines[line] << " ";
                }
                std::cout << std::endl;
            }
        }

        bool game_main() {
            std::cout << CLEAR;
            int dealer_score;
            int player_score;

            const std::string notice = "Dealer must stand on 17 and must draw to 16.";
            std::cout
                << YELLOW << "[?] Shuffling the deck...\n" << RESET;

            shuffle_deck();
            std::cout
                << GREEN << "[i] The deck has been shuffled!\n" << RESET
                << std::endl;

            std::vector<Card> dealer_hand = {draw_card(), draw_card()};
            std::vector<Card> player_hand = {draw_card(), draw_card()};

            std::cout
                << RED << "Dealer's Hand:\n";

            render_cards(dealer_hand, true);

            std::cout
                << GREEN << "\nYour Hand:\n";

            render_cards(player_hand);

            std::cout
                << BLUE << "\n" << notice << "\n" << RESET
                << std::endl;

            bool player_turn = (calculate_score(player_hand) != 21);
            while (player_turn) {
                int choice;
                std::cout
                    << "What would you like to do?\n"
                    << YELLOW << "[1] " << RESET << "Hit\n"
                    << YELLOW << "[2] " << RESET << "Stand\n"
                    << BLUE << "> " << RESET;

                std::cin >> choice;
                switch (choice) {
                    case 1: {
                        std::cout << CLEAR;
                        player_hand.push_back(draw_card());

                        int score = calculate_score(player_hand);

                        if (score == 21) {
                            player_turn = false;
                            break;
                        }

                        if (score > 21) {
                            std::cout
                                    << RED << "Dealer's Hand:\n";

                            render_cards(dealer_hand);

                            std::cout
                                    << GREEN << "\nYour Hand:\n";

                            render_cards(player_hand);

                            std::cout
                                    << RED << "\n\nYou busted! Your score is "
                                    << BLUE << score
                                    << RED << ".\n" << RESET;
                            return false;
                        }

                        std::cout
                                << RED << "Dealer's Hand:\n";

                        render_cards(dealer_hand, true);

                        std::cout
                                << GREEN << "\nYour Hand:\n";

                        render_cards(player_hand);
                        break;
                    }

                    case 2: {
                        player_turn = false;
                        break;
                    }

                    default: {
                        std::cout
                            << RED << "Invalid choice. Please try again.\n" << RESET;
                        break;
                    }
                }
            }

            player_score = calculate_score(player_hand);
            dealer_score = calculate_score(dealer_hand);
            if (player_score == 21 && dealer_score != 21) {
                std::cout << CLEAR;
                std::cout
                    << RED << "Dealer's Hand:\n";

                render_cards(dealer_hand);

                std::cout
                    << GREEN << "\nYour Hand:\n";

                render_cards(player_hand);

                std::cout
                    << GREEN << "\n\nYou win! Your score is "
                    << BLUE << player_score
                    << GREEN << " and the dealer's score is "
                    << BLUE << dealer_score
                    << GREEN << ".\n" << RESET;
                return true;
            }

            bool dealer_turn = true;
            while (dealer_turn) {
                std::cout << CLEAR;
                int score = calculate_score(dealer_hand);

                if (score >= 17) {
                    dealer_turn = false;
                    continue;
                } else {
                    dealer_hand.emplace_back(draw_card());

                    score = calculate_score(dealer_hand);
                    if (score > 21) {
                        std::cout
                            << RED << "Dealer's Hand:\n";

                        render_cards(dealer_hand);

                        std::cout
                            << GREEN << "\nYour Hand:\n";

                        render_cards(player_hand);

                        std::cout
                            << GREEN << "\n\nYou win! Dealer busted! Their score is "
                            << BLUE << score
                            << GREEN << ".\n" << RESET;
                        return true;
                    }
                }
            }

            std::cout
                << RED << "Dealer's Hand:\n";

            render_cards(dealer_hand);

            std::cout
                << GREEN << "\nYour Hand:\n";

            render_cards(player_hand);

            player_score = calculate_score(player_hand);
            dealer_score = calculate_score(dealer_hand);

            if (player_score > dealer_score) {
                std::cout
                    << GREEN << "\n\nYou win! Your score is "
                    << BLUE << player_score
                    << GREEN << " and the dealer's score is "
                    << BLUE << dealer_score
                    << GREEN << ".\n" << RESET;
                return true;
            } else if (dealer_score > player_score) {
                std::cout
                    << RED << "\n\nYou lost. Your score is "
                    << BLUE << player_score
                    << RED << " and the dealer's score is "
                    << BLUE << dealer_score
                    << RED << ".\n" << RESET;
                return false;
            } else if (dealer_score == player_score) {
                std::cout
                    << GREEN << "\n\nIt's a tie! Your score is "
                    << BLUE << player_score
                    << GREEN << " and the dealer's score is "
                    << BLUE << dealer_score
                    << GREEN << ".\n" << RESET;
                return true;
            }

            return false;
        }
    }

    namespace guess_the_number {
        bool game_main() {
            std::cout << CLEAR;
            short winning_number;
            short guess;

            std::cout
                    << RED <<   "====================================\n" << RESET
                    << RED <<   "||  Welcome to Guess the Number!  ||\n" << RESET
                    << RED <<   "====================================\n" << RESET
                    << std::endl;

            std::cout
                    << YELLOW << "[?] Getting the game ready for you...\n" << RESET;

            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<short> dist(0, 100);

            winning_number = dist(rng);

            std::cout
                    << GREEN << "[i] The game is ready!\n" << RESET
                    << std::endl;

            std::cout
                    << "What is your guess? (Max: 100)\n"
                    << BLUE << "> " << RESET;

            std::cin >> guess;

            std::cout
                    << BLUE << "You guessed: " << RESET
                    << guess << std::endl;


            if (guess == winning_number) {
                std::cout
                        << GREEN << "\n\nCongratulations! You guessed the number!\n" << RESET;

                return true;
            } else {
                std::cout
                        << RED << "\n\nYou lost. "
                        << GREEN << "The winning number was "
                        << BLUE << winning_number
                        << GREEN << ".\n" << RESET;

                return false;
            }
        }
    }
}


const std::vector<std::string> menu = {
        "Guess the Number",
        "Roulette",
        "Slots",
        "Blackjack",
        "Exit"
};

int get_menu_choice(const std::vector<std::string> &choices) {
    std::cout
            << RED <<   "==============================\n" << RESET
            << RED <<   "||  Welcome to the Casino!  ||\n" << RESET
            << RED <<   "==============================\n" << RESET
            << std::endl;

    int choice;
    int idx = 0;
    for (const std::string &option : choices) {
        idx++;
        std::cout
            << YELLOW <<  "[" << idx << "] " << RESET
            << option << std::endl;
    }

    std::cout
        << "What would you like to play? \n"
        << BLUE << "> " << RESET;
    std::cin >> choice;
    if (choice < 1 || choice > choices.size()) {
        std::cout << CLEAR;
        return get_menu_choice(choices);
    }
    return choice;
}

void await_resume() {
    std::cout
        << BLUE << "Press <ENTER> to continue playing...\n" << RESET;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    while (true) {
        std::cout << CLEAR;

        int choice = get_menu_choice(menu);

        switch (choice) {
            case 1: {
                games::guess_the_number::game_main();
                await_resume();
                break;
            }

            case 2: {
                break;
            }

            case 3: {
                games::slots::game_main();
                await_resume();
                break;
            }

            case 4: {
                games::blackjack::game_main();
                await_resume();
                break;
            }

            case 5: {
                std::cout
                        << CLEAR
                        << GREEN << "\nThanks for playing! Goodbye!\n" << RESET;

                return EXIT_SUCCESS;
            }

            default: {
                std::cout
                        << RED << "Invalid choice. Please try again.\n" << RESET;

                break;
            }
        }
    }
}
