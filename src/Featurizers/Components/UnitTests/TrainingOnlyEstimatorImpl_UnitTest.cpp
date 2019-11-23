// ----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License
// ----------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../../TestHelpers.h"
#include "../TrainingOnlyEstimatorImpl.h"

namespace NS = Microsoft::Featurizer;

template <typename T>
class CountEstimatorPolicyBase {
public:
    // ----------------------------------------------------------------------
    // |  Public Types
    using InputType                         = T;

    struct Results {
        using CountMap                      = std::map<T, std::uint32_t>;

        CountMap const                      Counts;

        Results(CountMap counts) : Counts(std::move(counts)) {}

        FEATURIZER_MOVE_CONSTRUCTOR_ONLY(Results);
    };

    // ----------------------------------------------------------------------
    // |  Public Data
    static constexpr char const * const     NameValue = "Test";

    // ----------------------------------------------------------------------
    // |  Public Methods
    Results complete_training(void) {
        return Results(std::move(_counts));
    }

protected:
    // ----------------------------------------------------------------------
    // |  Protected Data
    typename Results::CountMap              _counts;

};

/////////////////////////////////////////////////////////////////////////
///  \class         SingleCountEstimatorPolicy
///  \brief         Counts distinct values.
///
template <typename T>
class SingleCountEstimatorPolicy : public CountEstimatorPolicyBase<T> {
public:
    // ----------------------------------------------------------------------
    // |  Public Types
    using BaseType                          = CountEstimatorPolicyBase<T>;

    // ----------------------------------------------------------------------
    // |  Public Methods
    template <typename U>
    void fit(U const &input) {
        std::uint32_t &                     count(
            [this, &input](void) -> std::uint32_t & {
                typename BaseType::Results::CountMap::iterator const        i(BaseType::_counts.find(input));

                if(i != BaseType::_counts.end())
                    return i->second;

                std::pair<typename BaseType::Results::CountMap::iterator, bool> const   result(BaseType::_counts.emplace(std::make_pair(input, 0)));

                return result.first->second;
            }()
        );

        count += 1;
    }
};

template <typename T>
class MultipleCountEstimatorPolicy : public CountEstimatorPolicyBase<T> {
public:
    // ----------------------------------------------------------------------
    // |  Public Types
    using BaseType                          = CountEstimatorPolicyBase<T>;

    // ----------------------------------------------------------------------
    // |  Public Methods
    template <typename U>
    void fit(U const *pItems, size_t cItems) {
        U const * const                     pEndItems(pItems + cItems);

        while(pItems != pEndItems) {
            U const &                       input(*pItems++);

            std::uint32_t &                 count(
                [this, &input](void) -> std::uint32_t & {
                    typename BaseType::Results::CountMap::iterator const    i(BaseType::_counts.find(input));

                    if(i != BaseType::_counts.end())
                        return i->second;

                    std::pair<typename BaseType::Results::CountMap::iterator, bool> const   result(BaseType::_counts.emplace(std::make_pair(input, 0)));

                    return result.first->second;
                }()
            );

            count += 1;
        }
    }
};

template <
    typename EstimatorPolicyT,
    size_t MaxNumTrainingItemsV=std::numeric_limits<size_t>::max()
>
std::map<typename EstimatorPolicyT::InputType, std::uint32_t> Test(
    std::vector<std::vector<typename EstimatorPolicyT::InputType>> const &inputBatches
) {
    NS::AnnotationMapsPtr                                                                           pAllColumnAnnotations(NS::CreateTestAnnotationMapsPtr(1));
    NS::Featurizers::Components::TrainingOnlyEstimatorImpl<EstimatorPolicyT, MaxNumTrainingItemsV>  estimator(pAllColumnAnnotations, 0);

    NS::TestHelpers::Train(estimator, inputBatches);

    typename EstimatorPolicyT::Results const &          results(estimator.get_annotation_data());

    return results.Counts;
}

TEST_CASE("Ints - single") {
    std::map<int, std::uint32_t> const      counts(Test<SingleCountEstimatorPolicy<int>>({{1, 2, 3}, {4, 5}, {3, 5}}));

    CHECK(counts == std::map<int, std::uint32_t>{{1, 1}, {2, 1}, {3, 2}, {4, 1}, {5, 2}});
}

TEST_CASE("Ints - multiple") {
    std::map<int, std::uint32_t> const      counts(Test<MultipleCountEstimatorPolicy<int>>({{1, 2, 3}, {4, 5}, {3, 5}}));

    CHECK(counts == std::map<int, std::uint32_t>{{1, 1}, {2, 1}, {3, 2}, {4, 1}, {5, 2}});
}

TEST_CASE("Limited Ints - single") {
    std::map<int, std::uint32_t> const      counts(Test<SingleCountEstimatorPolicy<int>, 4>({{1, 2, 3}, {4, 5}, {3, 5}}));

    CHECK(counts == std::map<int, std::uint32_t>{{1, 1}, {2, 1}, {3, 1}, {4, 1}});
}

TEST_CASE("Limited Ints - multiple") {
    std::map<int, std::uint32_t> const      counts(Test<MultipleCountEstimatorPolicy<int>, 4>({{1, 2, 3}, {4, 5}, {3, 5}}));

    CHECK(counts == std::map<int, std::uint32_t>{{1, 1}, {2, 1}, {3, 1}, {4, 1}});
}

TEST_CASE("Strings - single") {
    std::map<std::string, std::uint32_t> const          counts(Test<SingleCountEstimatorPolicy<std::string>>({{"one", "two", "three"}, {"three", "two"}, {"four"}}));

    CHECK(counts == std::map<std::string, std::uint32_t>{{"one", 1}, {"two", 2}, {"three", 2}, {"four", 1}});
}

TEST_CASE("Strings - multiple") {
    std::map<std::string, std::uint32_t> const          counts(Test<MultipleCountEstimatorPolicy<std::string>>({{"one", "two", "three"}, {"three", "two"}, {"four"}}));

    CHECK(counts == std::map<std::string, std::uint32_t>{{"one", 1}, {"two", 2}, {"three", 2}, {"four", 1}});
}
