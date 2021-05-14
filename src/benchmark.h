#pragma once

#include "sets.h"
#include "sorters.h"
#include "experiment.h"

#include <cmath>
#include <cfenv>
#include <cstdio>	// printf
#include <utility>
#include <fstream>
#include <filesystem>
#include <functional>	// bind

namespace benchmark {
	using algorithm_t = sorters::sorter_t<sets::iterator_t>;

	using timings_t = std::map<size_t, experiment::time_t>;

	enum step_generator_t { linear, quadratic };

	timings_t run(algorithm_t algorithm, sets::set_t set, int total_chunks,
		step_generator_t step_generator
	) {
		timings_t timings;

		const size_t set_size = set.size();

		std::fesetround(FE_TONEAREST);

		const size_t chunk_size = set_size > total_chunks ? (
			std::nearbyint((
				step_generator == quadratic ? sqrt(set_size) : set_size
			) / total_chunks)
		) : 1;

		printf("set_size: %d, chunk_size: %d\n", set_size, chunk_size);

		for (size_t i = 1; i <= total_chunks; i++) {
			auto subset_size = step_generator == quadratic ? (
				pow(chunk_size * i, 2)
			) : chunk_size * i;

			auto subset = sets::set_t(set.begin(), set.begin() + subset_size);
			const auto time = experiment(std::bind(algorithm,
				subset.begin(), subset.end(), std::less<>())
			).run();

			timings.emplace(i, time.count());
		}

		return timings;
	}

	// TODO: A class inheriting from std::ofstream with an overloaded <<
	// 	 operator would be significantly cleaner than this.
	// TODO: const char * should be std::string.
	void write(std::string sub_path, const char *algo_name, const char *set_name,
		timings_t timings) {
		std::filesystem::path file_path;

		if (sub_path.size())
			file_path += sub_path;

		file_path += "/";

		std::filesystem::create_directories(file_path);

		file_path += algo_name;
		file_path += "_";
		file_path += set_name;

		printf("writing %s\n", file_path.c_str());

		if (std::filesystem::exists(file_path)) {
			std::filesystem::remove(file_path);
		}

		std::ofstream os(file_path.c_str());

		for (const auto [n, t] : timings) {
			os << n << " " << t << "\n";
		}
	}
}
