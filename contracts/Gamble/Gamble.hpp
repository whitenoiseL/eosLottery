//
// Created by liyuankun on 11/14/18.
//
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>

#include <string>

namespace Gamble {

    using std::string;
    using namespace eosio;

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

        // @abi table player_info i64
        struct player_info {
            account_name player;
            asset    bet;
            uint64_t number;

            uint64_t primary_key()const { return player; }

            EOSLIB_SERIALIZE(player_info, (player)(bet)(number))
        };

        struct account {
            asset    balance;
            uint64_t primary_key()const { return balance.symbol.name(); }
        };

        struct isStart {
            bool status;
            uint64_t primary_key()const { return status; }
        };

        typedef eosio::multi_index<N(player_info), player_info> Player_info;
        typedef eosio::multi_index<N(accounts), account> accounts;
        typedef eosio::multi_index<N(isStart), isStart> IsStart;

        // @abi action
        void subbalance( account_name owner, asset value );
        // @abi action
        void addbalance( account_name owner, asset value, account_name ram_payer );

        uint64_t randomNumber();

    };

    EOSIO_ABI(gamble, (start)(announce)(play));
}
