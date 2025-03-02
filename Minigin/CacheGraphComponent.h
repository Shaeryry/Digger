#pragma once
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <chrono>
#include <any>

#include "Component.h"
#include "imgui_plot.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace dae {
	class CacheGraphComponent : public Component
	{
	public:
		CacheGraphComponent(GameObject* gameObject);
		virtual void Update() override;
	private: 

		enum class TypeGraph {
			intGraph,
			gameObjectGraph,
			gameObjectAltGraph
		};

		struct MeasureData {
			std::vector<float> durations;
		};

		struct GraphData {
			std::string graphName;
			TypeGraph graphType;

			MeasureData data;
			ImU32 color;
			ImGui::PlotConfig configuration;

			bool ready;
			bool calculating;
		};

		struct GraphWindowData {
			std::string name;
			std::vector<GraphData*> graphs;

			std::vector<const float*> data_durations;
			std::vector<unsigned int> data_colors;
			float maxElement;

			ImGui::PlotConfig combinedConfiguration;
			bool showCombined;

			int samples{ 1 };
		};

		struct transform {
			float matrix[16] = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		};

		class gameObject {
			public:
				gameObject& operator*=(int scalar) {
					id *= scalar;
					return *this;
				}

				transform local;
				int id;
		};

		class gameObjectAlt {
			public:
				gameObjectAlt& operator*=(int scalar) {
					id *= scalar;
					return *this;
				}

				transform* local;
				int id;
		};

        template<typename Container> MeasureData Measure(int samples = 1);
		template<typename Container> void Execute(Container& container, int stepSize);
		void GraphWindow(GraphWindowData& window);
		void UpdateWindow(GraphWindowData& window);
		ImGui::PlotConfig GetPlotConfiguration();
		ImGui::PlotConfig CreateCombinedPlot(GraphWindowData& window);


		GraphData m_Exercise1Data;
		GraphData m_Exercise2Data;
		GraphData m_Exercise3Data;

		GraphWindowData m_Window1;
		GraphWindowData m_Window2;

		int m_ArraySize;
		const int m_StepsCount{ 11 };
		const float m_StepsArray[ 11 ] = { 1,2,4,8,16,32,64,128,256,512,1024 };

	};

	template<typename Container>
	inline CacheGraphComponent::MeasureData CacheGraphComponent::Measure(int samples)
	{
		MeasureData returnData{};
		Container copyContainer(m_ArraySize);

		for (int stepIndex{ 0 }; stepIndex < m_StepsCount; stepIndex++) {

			std::vector<float> time{};
			int stepSize = static_cast<int>(m_StepsArray[stepIndex]);

			for (int sampleIndex{ 0 }; sampleIndex < samples; ++sampleIndex) {
				auto start = std::chrono::high_resolution_clock::now();

				Execute<Container>(copyContainer,stepSize); // Does the operation we want to measure !

				auto end = std::chrono::high_resolution_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				time.emplace_back(static_cast<float>(elapsedTime));
			};

			const float average = std::accumulate(time.begin(), time.end(), 0.0f) / static_cast<float>(time.size());
			returnData.durations.emplace_back(average);
		}

		return returnData;
	}

	template<typename Container>
	inline void CacheGraphComponent::Execute(Container& container, int stepSize)
	{
		for (int i{ 0 }; i < container.size(); i += stepSize)
		{
			container[i] *= 2;
		}
	}
}

