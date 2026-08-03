#pragma once

#include "../../core/state.hpp"
#include "../action.hpp"


namespace Actions::Cards {


  // Заполняет флаги Action, указывающие на возможные модификации карт
  // Используется на MCTS уровне 1 для информирования о возможных действиях на уровнях 2-3
  void fillCardFlagsForAction(Action& action, const State& state);


  // Функции проверки применимости карт к действию
  namespace CardApplicability {

    // Adrenaline: можно применить к любому Spent (not Fresh) юниту для любого действия
    bool canUseAdrenaline(const State& state, const Action& action);

    // CommandAction: можно применить к любому действию любого своего юнита
    bool canUseCommandAction(const State& state, const Action& action);

    // FollowMe: требует Hit Marker, CAPs 2, и работает с Rally
    bool canUseFollowMe(const State& state, const Action& action);

    // RallyUp: требует Hit Marker, нет CAPs, только Rally
    bool canUseRallyUp(const State& state, const Action& action);

    // FrontlineOfficer: можно применить к любому действию, CAPs 2
    bool canUseFrontlineOfficer(const State& state, const Action& action);

    // SniperFire: только для Attack, нет CAPs
    bool canUseSniperFire(const State& state, const Action& action);

    // SeekCover: для Move/Stall в местности с покрытием, нет CAPs
    bool canUseSeekCover(const State& state, const Action& action);

    // SwiftAction: бонусная карта, можно к любому действию, CAPs 1
    bool canUseSwiftAction(const State& state, const Action& action);

    // CarefulAiming: бонусная карта, только для Attack, нет CAPs
    bool canUseCarefulAiming(const State& state, const Action& action);

    // Luck: после проверки броска (обрабатывается в applyAction), нет CAPs
    bool canUseLuck(const State& state, const Action& action);

    // VeteranNCO: бонусная карта, специальная логика, нет CAPs
    bool canUseVeteranNCO(const State& state, const Action& action);

    // BattlefieldConfusion: бонусная карта, специальная логика, нет CAPs
    bool canUseBattlefieldConfusion(const State& state, const Action& action);

    // RapidDeployment: бонусная карта, для Move, нет CAPs
    bool canUseRapidDeployment(const State& state, const Action& action);
  }


  // Проверяет, есть ли карта в руке у текущего игрока
  bool hasCardInHand(const State& state, Card card);


}
