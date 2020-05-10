#pragma once

#include <utility>
#include <functional>
#include<iostream>
#include <random>
#include <algorithm>


namespace go
{
	namespace internal
	{
		template<typename...> constexpr bool dependent_false = false;
	}

	// Select statements references: https://golang.org/ref/spec#Select_statements
	class Case
	{
		std::function<bool()> task;
	public:
		bool condition;
		template<typename func>
		Case(bool c, func f)
		{
			condition = c;
			task = [=]() {
				f();
				return true;
			};
		}

		bool operator() ()
		{
				return task();
		}
	};

	class Default
	{
		std::function<void()> task;
	public:
		template<typename func>
		Default(func f)
		{
			task = f;
		}

		void operator() ()
		{
			task();
		}
	};

	class Select
	{
		std::vector<Case> cases;

		bool randomExec()
		{
			for (auto& c : cases)
			{
				if (c.condition) {
					c();
					return true;
				}
			}
			return false;
		}

		template<typename ...T>
		void exec(Case&& c, T&&... params)
		{
			cases.emplace_back(c);
			exec(std::forward<T>(params)...);
		}

		void exec(Case&& c)
		{
			cases.emplace_back(c);
			randomExec();
		}

		void exec(Default && d)
		{
			if(!randomExec())
				d();
		}
		template<typename ...T>
		void exec(Default && c, T &&... params)
		{
			static_assert(internal::dependent_false<T...>, "There should only be at most 1 Default case and it must be the last parameter of the Select");
		}

	public:
		template<typename ...T>
		Select(T &&... params)//:cases(sizeof...(params))
		{
			cases.reserve(sizeof...(params));
			exec(std::forward<T>(params)...);
		}
	};
}

