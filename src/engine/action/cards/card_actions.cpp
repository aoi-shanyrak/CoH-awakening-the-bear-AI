#include <algorithm>

#include "card_actions.hpp"


using namespace Actions;
using namespace Cards;
using namespace CardsImpl;


namespace Actions::Cards {


  void addCardActions(const GenerationContext& context) {
    const auto& state {context.state};
    const auto& hand {(state.turn == Nation::SovietUnion) ? state.soviet.getHand() : state.germany.getHand()};

    std::sort(hand.begin(), hand.end());
    auto last {std::unique(hand.begin(), hand.end())};

    for (auto it {hand.begin()}; it != last; ++it) {
      switch (*it) {
        case Card::Adrenaline: addAdrenaline(context); break;
        case Card::CommandAction: addCommandAction(context); break;
        case Card::FollowMe: addFollowMe(context); break;
        case Card::RallyUp: addRallyUp(context); break;
        case Card::FrontlineOfficer: addFrontlineOfficer(context); break;
        case Card::SeekCover:
        case Card::SniperFire: addSniperFire(context); break;
        default: break;
      }
    }
  }


  namespace CardsImpl {


    void addAdrenaline(const GenerationContext& context) {
      std::vector<Action> actions {};
      for (const Action& base : context.actions) {
        if (base.type == ActionType::Pass) continue;
        const Unit& unit {context.state.units[base.unitIndex]};
        if (unit.isFresh()) continue;
        Action new_action {base};
        new_action.type = ActionType::PlayCard;
        new_action.base_action = base.type;
        new_action.card = Card::Adrenaline;
        new_action.cost = {0, 0};
        actions.push_back(new_action);
      }
      context.actions += actions;
    }

    void addCommandAction(const GenerationContext& context) {
      std::vector<Action> actions {};
      for (const Action& base : context.actions) {
        if (base.type == ActionType::Pass) continue;
        Action new_action {base};
        new_action.type = ActionType::PlayCard;
        new_action.base_action = base.type;
        new_action.card = Card::CommandAction;
        new_action.cost = {0, 0};
        actions.push_back(new_action);
      }
      context.actions += actions;
    }

    void addFollowMe(const GenerationContext& context) {
      for (uint8_t unitIdx {0}; unitIdx < context.state.units.size(); ++unitIdx) {
        const Unit& unit {context.state.units[unitIdx]};
        if (!unit.hasHitmarker() || unit.getNation() != context.state.turn) continue;
        context.actions.push_back(
            {ActionType::PlayCard, {2, 0}, unitIdx, {}, {}, {}, Card::FollowMe, ActionType::Rally});
      }
    }

    void addRallyUp(const GenerationContext& context) {
      for (uint8_t unitIdx {0}; unitIdx < context.state.units.size(); ++unitIdx) {
        const Unit& unit {context.state.units[unitIdx]};
        if (!unit.hasHitmarker() || unit.getNation() != context.state.turn ||
            isAnyEnemyUnitInHex(context, unit.getPosition()))
          continue;
        context.actions.push_back(
            {ActionType::PlayCard, {0, 0}, unitIdx, {}, {}, {}, Card::RallyUp, ActionType::Rally});
      }
    }

    void addFrontlineOfficer(const GenerationContext& context) {
      for (uint8_t unitIdx {0}; unitIdx < context.state.units.size(); ++unitIdx) {
        const Unit& unit {context.state.units[unitIdx]};
        if (unit.getNation() != context.state.turn) continue;
        context.actions.push_back({ActionType::PlayCard, {2, 0}, unitIdx, {}, {}, {}, Card::FrontlineOfficer});
      }
    }

    void addSniperFire(const GenerationContext& context) {
      for (uint8_t unitIdx {0}; unitIdx < context.state.units.size(); ++unitIdx) {
        const Unit& unit {context.state.units[unitIdx]};
        if (unit.getNation() != context.state.turn) continue;
        context.actions.push_back({ActionType::PlayCard, {0, 0}, unitIdx, {}, {}, {}, Card::SniperFire});
      }
    }


  }


}
