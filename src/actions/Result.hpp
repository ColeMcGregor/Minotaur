#pragma once
#include <string>
#include <vector>
#include <utility>

namespace minotaur {

/** A small record of what happened (for logs/UI). */
struct Outcome {
    std::string kind;    //  "Move", "Teleport", "ConsumeItem", "AP"
    std::string detail;  //  "to (4,3)", "spent 1 AP"
};

/** Verdict from validating/applying an Action. */
struct Result {
    bool success = true;
    std::string message;
    std::vector<Outcome> outcomes;

    // helpers
    static Result succeed(std::string msg = {}) {
        Result r; r.success = true; r.message = std::move(msg); return r;
    }
    static Result fail(std::string msg) {
        Result r; r.success = false; r.message = std::move(msg); return r;
    }

    Result& add(std::string kind, std::string detail = {}) {
        outcomes.push_back(Outcome{std::move(kind), std::move(detail)});
        return *this;
    }

    Result& merge(const Result& other) {
        outcomes.insert(outcomes.end(), other.outcomes.begin(), other.outcomes.end());
        return *this;
    }
};

} 
