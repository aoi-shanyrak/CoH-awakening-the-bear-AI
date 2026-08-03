#include "card_utils.hpp"
#include "../action.hpp"
#include "card.hpp"


using namespace Actions;
using namespace Cards;
using namespace CardApplicability;


namespace Actions::Cards {


  void fillCardFlagsForAction(Action& action, const State& state) {
    // Пропускаем Pass
    if (action.type == ActionType::Pass) return;

    const Unit& unit = state.units[action.unitIndex];

    // Определяем какие карты можно использовать для этого действия
    action.canSpendAdditionalCAPs = (action.cost.action_points > 0 && unit.isFresh());
    action.canUseAdrenaline = canUseAdrenaline(state, action);
    action.canUseCommandAction = canUseCommandAction(state, action);
    action.canUseFollowMe = canUseFollowMe(state, action);
    action.canUseRallyUp = canUseRallyUp(state, action);
    action.canUseFrontlineOfficer = canUseFrontlineOfficer(state, action);
    action.canUseSniperFire = canUseSniperFire(state, action);
    action.canUseSeekCover = canUseSeekCover(state, action);
    action.canUseSwiftAction = canUseSwiftAction(state, action);
    action.canUseCarefulAiming = canUseCarefulAiming(state, action);
    action.canUseLuck = canUseLuck(state, action);
    action.canUseVeteranNCO = canUseVeteranNCO(state, action);
    action.canUseBattlefieldConfusion = canUseBattlefieldConfusion(state, action);
    action.canUseRapidDeployment = canUseRapidDeployment(state, action);
  }


  namespace CardApplicability {

    bool canUseAdrenaline(const State& state, const Action& action) {
      if (action.type == ActionType::Pass) return false;
      if (!hasCardInHand(state, Card::Adrenaline)) return false;

      const Unit& unit = state.units[action.unitIndex];
      // Adrenaline только для Spent (не Fresh) юнитов
      return !unit.isFresh();
    }

    bool canUseCommandAction(const State& state, const Action& action) {
      if (action.type == ActionType::Pass) return false;
      return hasCardInHand(state, Card::CommandAction);
    }

    bool canUseFollowMe(const State& state, const Action& action) {
      if (action.type != ActionType::Rally) return false;
      if (!hasCardInHand(state, Card::FollowMe)) return false;

      const Unit& unit = state.units[action.unitIndex];
      // FollowMe требует Hit Marker и стоит 2 CAPs
      return unit.hasHitmarker();
    }

    bool canUseRallyUp(const State& state, const Action& action) {
      if (action.type != ActionType::Rally) return false;
      if (!hasCardInHand(state, Card::RallyUp)) return false;

      const Unit& unit = state.units[action.unitIndex];
      // RallyUp требует Hit Marker
      return unit.hasHitmarker();
    }

    bool canUseFrontlineOfficer(const State& state, const Action& action) {
      if (action.type == ActionType::Pass) return false;
      // FrontlineOfficer можно применить к любому действию, стоит 2 CAPs
      return hasCardInHand(state, Card::FrontlineOfficer);
    }

    bool canUseSniperFire(const State& state, const Action& action) {
      if (action.type != ActionType::Attack) return false;
      // SniperFire только для Attack, нет CAPs
      return hasCardInHand(state, Card::SniperFire);
    }

    bool canUseSeekCover(const State& state, const Action& action) {
      if (action.type != ActionType::Move && action.type != ActionType::Stall) return false;
      if (!hasCardInHand(state, Card::SeekCover)) return false;

      // SeekCover работает в местности с покрытием
      const auto& terrain = state.map->get(state.units[action.unitIndex].getPosition()).getTerrain();
      return true;  // TODO: проверить, есть ли покрытие
    }

    bool canUseSwiftAction(const State& state, const Action& action) {
      if (action.type == ActionType::Pass) return false;
      // SwiftAction бонусная карта, стоит 1 САП
      return hasCardInHand(state, Card::SwiftAction);
    }

    bool canUseCarefulAiming(const State& state, const Action& action) {
      if (action.type != ActionType::Attack) return false;
      // CarefulAiming бонусная карта для Attack
      return hasCardInHand(state, Card::CarefulAiming);
    }

    bool canUseLuck(const State& state, const Action& action) {
      // Luck используется после проверки броска, общая механика
      return hasCardInHand(state, Card::Luck);
    }

    bool canUseVeteranNCO(const State& state, const Action& action) {
      // VeteranNCO бонусная карта, специальная логика
      return hasCardInHand(state, Card::VeteranNCO);
    }

    bool canUseBattlefieldConfusion(const State& state, const Action& action) {
      // BattlefieldConfusion бонусная карта
      return hasCardInHand(state, Card::BattlefieldConfusion);
    }

    bool canUseRapidDeployment(const State& state, const Action& action) {
      if (action.type != ActionType::Move) return false;
      // RapidDeployment бонусная карта для Move
      return hasCardInHand(state, Card::RapidDeployment);
    }
  }


  bool hasCardInHand(const State& state, Card card) {
    const auto& hand = (state.turn == Nation::SovietUnion) ? state.soviet.getHand() : state.germany.getHand();
    return std::find(hand.begin(), hand.end(), card) != hand.end();
  }


}
