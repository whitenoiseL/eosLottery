//
// Created by liyuankun on 11/14/18.
//
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include "../eosio.token/eosio.token.hpp"

#include <string>

namespace eosiosystem {
    class system_contract;
}

namespace Gamble {

    using std::string;
    using eosio::asset;

    class gamble : public contract {
    public:
        gamble( account_name self ):contract(self){}
        // @abi action
        void start();

        // @abi action
        void announce();

        // @abi action
        void play( account_name player,
                   asset        bet,
                   uint64_t     number);

    private:

        bool isStart = false;

        // @abi table player_info i64
        struct player_info {
            account_name player;
            asset    bet;
            uint64_t number;

            uint64_t primary_key()const { return player; }

            EOSLIB_SERIALIZE(player_info, (player)(bet)(number))
        };

        typedef eosio::multi_index<N(player_info), player_info> Player_info;

        // @abi action
        void subbalance( account_name owner, asset value );
        // @abi action
        void addbalance( account_name owner, asset value, account_name ram_payer );

        uint64_t randomNumber();

    }
    EOSIO_ABI(Gamble, (start)(announce)(play))
}
