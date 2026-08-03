#include "mcts.hpp"
#include "../action/cards/card_utils.hpp"
#include "../core/action_generation.hpp"
#include "../core/interface.hpp"
#include <algorithm>
#include <cmath>
#include <random>


using namespace MCTS;
using namespace Actions;
using namespace GameEngine;


namespace MCTS {


  double MCTSNode::ucb1(double explorationConstant) const {
    if (visits == 0) return std::numeric_limits<double>::infinity();

    double exploitation = getAverageReward();
    double exploration = explorationConstant * std::sqrt(std::log(parent->visits) / visits);

    return exploitation + exploration;
  }


  MCTSSearch::MCTSSearch(const State& initialState) : rootState(initialState) {
    root = new MCTSNode();
    root->level = Level::Base;
    root->action = Action {ActionType::Pass};  // плейсхолдер
  }


  MCTSSearch::~MCTSSearch() {
    // Рекурсивное удаление дерева (автоматическое через unique_ptr)
    delete root;
  }


  void MCTSSearch::runIteration() {
    if (!root) return;

    // 1. Выбор: спускаемся по дереву с использованием UCB1 или выбираем неразвёрнутый узел
    MCTSNode* selectedNode = selectNode(root);

    // 2. Расширение: если узел неразвёрнут, создаём детей
    State stateAtNode = rootState;
    // TODO: Пересчитаем state до выбранного узла (нужна функция для воспроизведения пути)

    if (!selectedNode->isExpanded() && !selectedNode->isTerminal()) {
      expandNode(selectedNode, stateAtNode);
    }

    // 3. Симуляция: если есть дети, берём одного; иначе симулируем из selectedNode
    MCTSNode* simulationNode = selectedNode;
    if (!selectedNode->children.empty()) {
      simulationNode = selectedNode->children[0].get();  // или random
    }

    double reward = simulate(stateAtNode);

    // 4. Обратное распространение
    backpropagate(simulationNode, reward);
  }


  MCTSNode* MCTSSearch::selectNode(MCTSNode* node) {
    // Спускаемся вниз по дереву, пока все дети не разработаны
    while (node->isExpanded()) {
      // Выбираем ребёнка по UCB1
      MCTSNode* child = selectUCB1Child(node);
      if (!child) return node;
      node = child;
    }
    return node;
  }


  void MCTSSearch::expandNode(MCTSNode* node, const State& state) {
    if (node->isTerminal()) return;

    switch (node->level) {
      case Level::Base: {
        // Создаём детей для всех базовых действий
        auto baseActions = getBaseActions(state);
        for (const auto& action : baseActions) {
          auto child = std::make_unique<MCTSNode>();
          child->parent = node;
          child->level = Level::CAPs;
          child->action = action;
          node->children.push_back(std::move(child));
        }
        break;
      }

      case Level::CAPs: {
        // Создаём детей для доступных CAPs (0, 1, 2)
        // Если нет возможности тратить CAPs, переходим на уровень Card с 0 CAPs

        const auto& player = (state.turn == Nation::SovietUnion) ? state.soviet : state.germany;
        int maxCAPs = player.getCAPs();

        // Определяем, можно ли тратить дополнительные CAPs для этого действия
        bool canSpendCAPs = node->action.canSpendAdditionalCAPs && node->action.cost.action_points > 0;

        for (int8_t capsSpent = 0; capsSpent <= 2; ++capsSpent) {
          // Пропускаем варианты, которые недоступны
          if (capsSpent > 0 && !canSpendCAPs) break;
          if (capsSpent > maxCAPs) break;

          auto child = std::make_unique<MCTSNode>();
          child->parent = node;
          child->level = Level::Card;
          child->action = node->action;
          child->capsSpent = capsSpent;
          node->children.push_back(std::move(child));
        }

        // Если нет детей, создаём одного с 0 CAPs
        if (node->children.empty()) {
          auto child = std::make_unique<MCTSNode>();
          child->parent = node;
          child->level = Level::Card;
          child->action = node->action;
          child->capsSpent = 0;
          node->children.push_back(std::move(child));
        }
        break;
      }

      case Level::Card: {
        // Создаём детей для доступных карт
        // Первый ребёнок - "без карты", остальные - конкретные карты

        std::vector<Card> applicableCards;

        // Проверяем все карты, которые можно применить
        if (node->action.canUseAdrenaline) applicableCards.push_back(Card::Adrenaline);
        if (node->action.canUseCommandAction) applicableCards.push_back(Card::CommandAction);
        if (node->action.canUseFollowMe) applicableCards.push_back(Card::FollowMe);
        if (node->action.canUseRallyUp) applicableCards.push_back(Card::RallyUp);
        if (node->action.canUseFrontlineOfficer) applicableCards.push_back(Card::FrontlineOfficer);
        if (node->action.canUseSniperFire) applicableCards.push_back(Card::SniperFire);
        if (node->action.canUseSeekCover) applicableCards.push_back(Card::SeekCover);
        if (node->action.canUseSwiftAction) applicableCards.push_back(Card::SwiftAction);
        if (node->action.canUseCarefulAiming) applicableCards.push_back(Card::CarefulAiming);
        if (node->action.canUseLuck) applicableCards.push_back(Card::Luck);
        if (node->action.canUseVeteranNCO) applicableCards.push_back(Card::VeteranNCO);
        if (node->action.canUseBattlefieldConfusion) applicableCards.push_back(Card::BattlefieldConfusion);
        if (node->action.canUseRapidDeployment) applicableCards.push_back(Card::RapidDeployment);

        // Создаём ребёнка "без карты"
        auto noCardChild = std::make_unique<MCTSNode>();
        noCardChild->parent = node;
        noCardChild->level = Level::Terminal;
        noCardChild->action = node->action;
        noCardChild->capsSpent = node->capsSpent;
        noCardChild->cardUsed = std::nullopt;
        node->children.push_back(std::move(noCardChild));

        // Создаём детей для каждой применимой карты
        for (const auto& card : applicableCards) {
          auto cardChild = std::make_unique<MCTSNode>();
          cardChild->parent = node;
          cardChild->level = Level::Terminal;
          cardChild->action = node->action;
          cardChild->capsSpent = node->capsSpent;
          cardChild->cardUsed = card;
          node->children.push_back(std::move(cardChild));
        }
        break;
      }

      case Level::Terminal:
        // Листовой узел не расширяется
        break;
    }
  }


  double MCTSSearch::simulate(const State& state) {
    // Простая симуляция: случайное разыгрывание до конца игры
    // TODO: Реализовать полную симуляцию с случайным выбором действий
    // Пока возвращаем случайное значение

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    return dis(gen);
  }


  void MCTSSearch::backpropagate(MCTSNode* node, double reward) {
    while (node != nullptr) {
      node->visits++;
      node->totalReward += reward;
      node = node->parent;
    }
  }


  MCTSNode* MCTSSearch::selectUCB1Child(MCTSNode* node) const {
    if (node->children.empty()) return nullptr;

    MCTSNode* bestChild = node->children[0].get();
    double bestValue = bestChild->ucb1(explorationConstant);

    for (size_t i = 1; i < node->children.size(); ++i) {
      double value = node->children[i]->ucb1(explorationConstant);
      if (value > bestValue) {
        bestValue = value;
        bestChild = node->children[i].get();
      }
    }

    return bestChild;
  }


  MCTSNode* MCTSSearch::selectBestChild(MCTSNode* node) const {
    if (node->children.empty()) return nullptr;

    MCTSNode* bestChild = node->children[0].get();
    double bestReward = bestChild->getAverageReward();

    for (size_t i = 1; i < node->children.size(); ++i) {
      double reward = node->children[i]->getAverageReward();
      if (reward > bestReward) {
        bestReward = reward;
        bestChild = node->children[i].get();
      }
    }

    return bestChild;
  }


  const MCTSNode* MCTSSearch::getBestAction() const {
    if (!root || root->children.empty()) return nullptr;

    // Возвращаем ребёнка с наибольшим средним вознаграждением (exploitation)
    MCTSNode* bestChild = root->children[0].get();
    double bestReward = bestChild->getAverageReward();

    for (size_t i = 1; i < root->children.size(); ++i) {
      double reward = root->children[i]->getAverageReward();
      if (reward > bestReward) {
        bestReward = reward;
        bestChild = root->children[i].get();
      }
    }

    return bestChild;
  }


  Actions::Action assembleFullAction(const MCTSNode* node) {
    // Собираем полное действие из пути в дереве (цепочка Base -> CAPs -> Card -> Terminal)
    if (!node || node->level != Level::Terminal) {
      return Action {ActionType::Pass};  // fallback
    }

    Action fullAction = node->action;
    fullAction.card = node->cardUsed;
    fullAction.cost.needs_CAPs = node->capsSpent;

    return fullAction;
  }


}
