#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "../../engine/core/interface.hpp"


namespace MCTS {


  // Уровни иерархического дерева решений
  enum class Level : uint8_t {
    Base,  // Уровень 1: выбор базового действия (Move, Attack, Rally, Stall, Pass)
    CAPs,  // Уровень 2: выбор дополнительных CAPs (0, 1, 2)
    Card,  // Уровень 3: выбор карты для использования (или "без карты")
    Terminal  // Листовой узел - готовое действие к применению
  };


  // Узел иерархического дерева MCTS
  struct MCTSNode {
    // Структура дерева
    MCTSNode* parent = nullptr;
    std::vector<std::unique_ptr<MCTSNode>> children;
    Level level = Level::Base;

    // Данные для текущего уровня
    Actions::Action action;  // базовое действие (уровень Base)
    int8_t capsSpent = 0;  // дополнительные потраченные CAPs (уровень CAPs)
    std::optional<Card> cardUsed = std::nullopt;  // использованная карта (уровень Card)

    // Статистика для UCB1
    uint32_t visits = 0;
    double totalReward = 0.0;

    // Вспомогательные методы
    bool isExpanded() const { return !children.empty(); }
    bool isTerminal() const { return level == Level::Terminal; }
    double getAverageReward() const { return visits > 0 ? totalReward / visits : 0.0; }

    // UCB1 формула для выбора узла
    // explorationConstant обычно ~1.414 (sqrt(2))
    double ucb1(double explorationConstant) const;
  };


  // MCTS поиск
  class MCTSSearch {
   private:
    MCTSNode* root = nullptr;
    State rootState;
    double explorationConstant = 1.414;

   public:
    explicit MCTSSearch(const State& initialState);
    ~MCTSSearch();

    // Основной цикл MCTS: выбор -> расширение -> симуляция -> обновление
    void runIteration();

    // Возвращает лучшее действие по UCB1 или по среднему вознаграждению
    const MCTSNode* getBestAction() const;

    // Возвращает корневой узел
    MCTSNode* getRootNode() const { return root; }

    // Статистика поиска
    uint32_t getTotalVisits() const { return root ? root->visits : 0; }
    double getAverageRootReward() const { return root ? root->getAverageReward() : 0.0; }

   private:
    // Шаги MCTS алгоритма
    MCTSNode* selectNode(MCTSNode* node);
    void expandNode(MCTSNode* node, const State& state);
    double simulate(const State& state);
    void backpropagate(MCTSNode* node, double reward);

    // Вспомогательные функции
    MCTSNode* selectBestChild(MCTSNode* node) const;
    MCTSNode* selectUCB1Child(MCTSNode* node) const;
  };


  // Конструктор для применения действия MCTS уровней 2-3
  Actions::Action assembleFullAction(const MCTSNode* node);


}
