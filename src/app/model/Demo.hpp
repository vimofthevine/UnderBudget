#pragma once

// Standard include(s)
#include <memory>

// UnderBudget include(s)
#include "Repositories.hpp"

namespace ub {

/**
 * Populates the repository with demo data.
 */
class Demo {
public:
    /**
     * Populates the given repositories with demo data.
     *
     * @param repos Repositories
     */
    static void populate(std::shared_ptr<Repositories> repos);
};

} // ub namespace
