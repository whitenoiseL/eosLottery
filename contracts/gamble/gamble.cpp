//
// Created by liyuankun on 11/14/18.
//

#include "gamble.hpp"

namespace Gamble {

    void gamble::start() {
        eosio_assert(!gamble::isStart, "The game has already started!");
        gamble::isStart = true;
    }

    void gamble::announce() {
        eosio_assert(gamble::isStart,"No game found!");

        uint64_t goal = randomNumber();
        uint64_t total = 0

        Player_info info(_self, _self);

        for(auto iter = info.begin();iter!=info.end();iter++){
            auto current_player = iter.player;
            auto current_number = iter.number;

            if (current_number == goal){
                total += iter.bet.amount;
            } else {
                info.erase(iter);
            }
        };

        for(auto iter = info.begin();iter!=info.end();iter++){

            symbol_type symbol(S(4,TNT));
            const auto& ac = accountstable.get(symbol.name());
            auto pool = ac.amount;

            auto prize_share = iter.bet.amount/total*pool;
            asset prize = asset(prize_share, string_to_symbol(4, "TNT"));
            action(
                permission_level{_self,N(active)},
                N(token), N(transfer),
                std::make_tuple(_self, iter.player, prize, string("prize"))
            ).send();
            info.erase(iter);
        };

        gamble::isStart = false;
    }

    void gamble::play(account_name player, asset bet, uint64_t number) {
        require_auth(account);
        eosio_assert(gamble::isStart, "The game is not started yet!");
        eosio_assert(bet.amount >= 2, "Minimum bet is 2!")

        print("play function, self is: ", name{_self});
        Player_info info(_self, _self);

        auto iterator = info.find(player);
        eosio_assert(iterator == info.end(), "You are in game!");

        info.emplace(player, [&](auto& player){
           info.player = player;
           info.bet = bet;
           info.number = number;
        });

        action(
            permission_level{player, N(active)},
            N(token), N(transfer),
            std::make_tuple(player, _self, bet, string("bet"))
        ).send();

    }

    void gamble::randomNumber() {
        return rand() % 11
    }



}
