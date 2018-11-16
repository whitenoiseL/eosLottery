//
// Created by liyuankun on 11/14/18.
//

#include "Gamble.hpp"

namespace Gamble {

    void gamble::start() {
        require_auth(_self);
        IsStart status(_self,_self);
        status.emplace(_self, [&](auto &s){
            s.status = true;
        });

        print("Game start status: ", status.begin()->status);
    };

    void gamble::announce() {
        require_auth(_self);
        IsStart status(_self,_self);
        auto stat = status.begin();
        eosio_assert(stat->status, "No game found!");

        uint64_t goal = randomNumber();
        print("Goal number is: ", goal);
        uint64_t total = 0;

        Player_info info(_self, _self);

        for(auto iter = info.begin();iter!=info.end();){
            auto current_player = iter->player;
            auto current_number = iter->number;

            if (current_number == goal){
                total += iter->bet.amount;
                iter++;
            } else {
                iter = info.erase(iter);
            }
        };

        print("total: ", total);

        for(auto iter = info.begin();iter!=info.end();){

            symbol_type symbol(S(4,TNT));
            accounts fromAcc(N(eosio.token), _self);
            auto Acc = fromAcc.get(symbol.name());
            auto pool = Acc.balance.amount;
            auto prize_share = iter->bet.amount*pool/total;
            asset prize = asset(prize_share, string_to_symbol(4, "TNT"));
            action(
                permission_level{_self,N(active)},
                N(eosio.token), N(transfer),
                std::make_tuple(_self, iter->player, prize, string("prize"))
            ).send();
            iter = info.erase(iter);
        };

        status.erase(stat);
    };

    void gamble::play(account_name player, asset bet, uint64_t number) {
        require_auth(player);
        IsStart status(_self,_self);
        auto stat = status.begin();
        print("Game status: ", stat->status);
        eosio_assert(stat->status, "No game found!");
        eosio_assert(bet.amount >= 2, "Minimum bet is 2!");

        print("play function, self is: ", name{_self});
        Player_info info(_self, _self);

        auto iterator = info.find(player);
        eosio_assert(iterator == info.end(), "You are in game!");

        info.emplace(player, [&](auto& i){
           i.player = player;
           i.bet = bet;
           i.number = number;
        });

        action(
            permission_level{player, N(active)},
            N(eosio.token), N(transfer),
            std::make_tuple(player, _self, bet, string("bet"))
        ).send();

    };

    uint64_t gamble::randomNumber() {
        checksum256 result;
        auto mixedBlock = tapos_block_prefix() * tapos_block_num();

        const char *mixedChar = reinterpret_cast<const char *>(&mixedBlock);
        sha256( (char *)mixedChar, sizeof(mixedChar), &result);
        const char *p64 = reinterpret_cast<const char *>(&result);
        return (uint64_t(*p64)) % 11;
    }
}
