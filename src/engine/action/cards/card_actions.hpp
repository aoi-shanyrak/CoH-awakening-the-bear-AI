#pragma once

#include "../../core/action_generation.hpp"


namespace Actions::Cards {


  void addCardActions(const GenerationContext& context);


  namespace CardsImpl {


    void addAdrenaline(const GenerationContext& context);
    void addCommandAction(const GenerationContext& context);
    void addFollowMe(const GenerationContext& context);
    void addRallyUp(const GenerationContext& context);
    void addFrontlineOfficer(const GenerationContext& context);
    void addSniperFire(const GenerationContext& context);


  }


}
