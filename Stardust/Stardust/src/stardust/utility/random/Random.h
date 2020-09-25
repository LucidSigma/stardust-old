#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include "../interfaces/INoncopyable.h"
#include "../interfaces/INonmovable.h"

#include <concepts>
#include <cstddef>
#include <random>
#include <vector>

namespace stardust
{
	class Random
		: private INoncopyable, private INonmovable
	{
	public:
		enum class BinomialDistributionType
		{
			Binomial,
			NegativeBinomial,
		};

	private:
		inline static std::mt19937 s_randomEngine{ std::random_device()() };

	public:
		static void ReSeed();

		template <std::floating_point T = float>
		[[nodiscard]] static T GenerateFloat(const T min = 0.0f, const T max = 1.0f)
		{
			const std::uniform_real_distribution<T> uniformDistribution(min, max);

			return uniformDistribution(s_randomEngine);
		}

		template <std::integral T = int>
		[[nodiscard]] static T GenerateInt(const T min, const T max)
		{
			const std::uniform_int_distribution<T> uniformDistribution(min, max);

			return uniformDistribution(s_randomEngine);
		}

		[[nodiscard]] static bool FlipCoin();
		[[nodiscard]] static unsigned int RollDice(const unsigned int sideCount = 6u);

		[[nodiscard]] static bool BernoulliDistribution(const float probability);
		[[nodiscard]] static unsigned int BinomialDistribution(const unsigned int trials, const float probability, const BinomialDistributionType distributionType = BinomialDistributionType::Binomial);
		[[nodiscard]] static unsigned int PoissonDistribution(const unsigned int frequency);
		
		template <std::floating_point T = float>
		[[nodiscard]] static T NormalDistribution(const T mean, const T standardDeviation)
		{
			std::normal_distribution<T> normalDistribution(mean, standardDeviation);

			return normalDistribution(s_randomEngine);
		}

		[[nodiscard]] static std::size_t DiscreteDistribution(const std::vector<double>& weights);

		Random() = delete;
		~Random() noexcept = delete;
	};
}

#endif