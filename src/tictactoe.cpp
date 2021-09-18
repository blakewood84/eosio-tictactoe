#include <eosio/eosio.hpp>
using namespace eosio;
using namespace std;

CONTRACT tictactoe : public contract {


  public:
    using contract::contract;

    ACTION welcome( name challenger, name host ) {
      check( has_auth( name("host") ), "You're not the host!" );
      print("Welcome challengers, ", host, " and ", challenger);
    }

    ACTION create( name challenger, name host) {
      // require host authorization
      check( has_auth( name("host") ), "You're not the host! You cannot create a new game" );

      games_table _games( get_self(), get_self().value );

      // check that the host and challenger aren't the same player
      // check that there isn't a game already between them
      if(challenger != host) {
        // if game doesn't exist create game

        auto itr = _games.find( host.value + challenger.value );
        check( itr == _games.end(), "A game already exists" );

        _games.emplace( get_self(), [&](auto& new_game_row) {
          new_game_row.game = host.value + challenger.value;
          new_game_row.host = host;
          new_game_row.challenger = challenger;
        });
        
      } else {
        check(0, "Host and Challenger cannot be the same person!");
      }

    }

    ACTION close( name challenger, name host) {
      check( has_auth( name("host") ), "You're not the host! You cannot close a game" );

      games_table _games( get_self(), get_self().value );
      
      auto itr = _games.find( host.value + challenger.value );
      check( itr != _games.end(), "There is no game for these two players" );
      _games.erase(itr);

    }

  private:
    TABLE games {
      uint64_t game;
      uint64_t bychallenger;
      name host;
      name challenger;

      auto primary_key() const { return game; }
      uint64_t get_secondary() const { return bychallenger;
    };
    typedef multi_index<name( "games" ), games, indexed_by<name("bychallenger") > games_table;
};
