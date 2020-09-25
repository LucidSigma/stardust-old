#include "Random.h"

#include <algorithm>
#include <iterator>

namespace stardust
{
	void Random::ReSeed()
	{
		s_randomEngine.seed(std::random_device()());
	}

	[[nodiscard]] bool Random::FlipCoin()
	{
		return GenerateInt(0, 1) == 0;
	}

	[[nodiscard]] unsigned int Random::RollDice(const unsigned int sideCount)
	{
		return GenerateInt<unsigned int>(1u, sideCount);
	}

	[[nodiscard]] bool Random::BernoulliDistribution(const float probability)
	{
		const std::bernoulli_distribution bernoulliDistribution(std::clamp(probability, 0.0f, 1.0f));

		return bernoulliDistribution(s_randomEngine);
	}

	[[nodiscard]] unsigned int Random::BinomialDistribution(const unsigned int trials, const float probability, const BinomialDistributionType distributionType)
	{
		switch (distributionType)
		{
		case BinomialDistributionType::NegativeBinomial:
		{
			const std::negative_binomial_distribution negativeNinomialDistribution(trials, std::clamp(probability, 0.0f, 1.0f));

			return negativeNinomialDistribution(s_randomEngine);
		}

		case BinomialDistributionType::Binomial:
		default:
		{
			const std::binomial_distribution binomialDistribution(trials, std::clamp(probability, 0.0f, 1.0f));

			return binomialDistribution(s_randomEngine);
		}
		}
	}

	[[nodiscard]] unsigned int Random::PoissonDistribution(const unsigned int frequency)
	{
		const std::poisson_distribution poissonDistribution(frequency);

		return poissonDistribution(s_randomEngine);
	}

	[[nodiscard]] std::size_t Random::DiscreteDistribution(const std::vector<double>& weights)
	{
		const std::discrete_distribution discreteDistribution(std::cbegin(weights), std::cend(weights));

		return discreteDistribution(s_randomEngine);
	}
}