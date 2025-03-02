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

enum class TypeGraph {
	intGraph,
	gameObjectGraph,
	gameObjectAltGraph
};

struct MeasureData {
	std::vector<float> steps;
	std::vector<float> durations;
};

struct GraphData{
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
	int samples{ 1 };

	ImGui::PlotConfig combinedConfiguration;
	bool showCombined;
};


namespace dae {
	class CacheGraphComponent : public Component
	{
	public:
		CacheGraphComponent(GameObject* gameObject);
		void SetStep(int step) { m_Step = step; };
		void SetMaxSteps(int steps) { m_MaxSteps = steps; };

		virtual void Update() override;
	private: 

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


		GraphData m_Exercise1Data;
		GraphData m_Exercise2Data;
		GraphData m_Exercise3Data;

		GraphWindowData m_Window1;
		GraphWindowData m_Window2;

		int m_Step;
		int m_MaxSteps;
		int m_ArraySize;
	};

	 
	template<typename Container>
	inline MeasureData CacheGraphComponent::Measure(int samples)
	{
		MeasureData returnData{};
		Container copyContainer(m_ArraySize);

		for (int stepSize{ 1 }; stepSize <= m_MaxSteps; stepSize *= m_Step) {

			std::vector<float> time{};

			for (int sampleIndex{ 0 }; sampleIndex < samples; ++sampleIndex) {
				auto start = std::chrono::high_resolution_clock::now();
				
				Execute<Container>(copyContainer, stepSize); // Does the operation we want to measure !

				auto end = std::chrono::high_resolution_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				time.emplace_back(static_cast<float>(elapsedTime));
			};

			const float average = std::accumulate(time.begin(), time.end(), 0.0f) / static_cast<float>(time.size());
			returnData.durations.emplace_back(average);
			returnData.steps.emplace_back(static_cast<float>(stepSize));
		}

		return returnData;
	}

	inline void CacheGraphComponent::GraphWindow(GraphWindowData& window)
	{
		auto graphs = window.graphs;

		ImGui::Begin(window.name.data());
		ImGui::InputInt("Samples", &window.samples, 0, 0);
		ImGui::SameLine(); if (ImGui::Button("+")) window.samples++;
		ImGui::SameLine(); if (ImGui::Button("-")) window.samples--;

		bool allReady{ true };
		for (int graphIndex{ 0 }; graphIndex < graphs.size(); graphIndex++) {
			GraphData& graph = *graphs[graphIndex];
 
			if (graph.ready) {
				ImGui::Plot("Graph" + graphIndex, graph.configuration);
			}

			if (graph.calculating) {
				// Data collection
				MeasureData result;

				switch (graph.graphType)
				{
					case TypeGraph::gameObjectGraph:
						result = Measure<std::vector<gameObject>>(window.samples);
						break;
					case TypeGraph::gameObjectAltGraph:
						result = Measure<std::vector<gameObjectAlt>>(window.samples);
						break;
					default:
						result = Measure<std::vector<int>>(window.samples);
						break;
				}

				graph.data = result;
				//window.combinedDurations.resize( graphs.size() );
				//window.combinedDurations.emplace_back( graph.data.durations ); // Put the adress of the data in the vector

				// Create PlotConfiguration

				const MeasureData& data = graph.data;
				const float* stepsArr = data.steps.data();
				const float* durationArr = data.durations.data();

				ImGui::PlotConfig conf;
				conf.values.xs = stepsArr;
				conf.values.ys = durationArr;

				conf.values.color = graph.color;
				conf.values.count = static_cast<int>(data.steps.size());

				conf.scale.min = 0.f;
				conf.scale.max = *std::max_element(durationArr, durationArr + data.durations.size());

				conf.tooltip.format = "x=%.0f, y=%.3f";
				conf.frame_size = ImVec2(150, 100);
				conf.line_thickness = 2.f;

				conf.tooltip.show = true;
				conf.grid_x.show = true;
				conf.grid_y.show = true;

				graph.configuration = conf;

				//

				graph.calculating = false;
				graph.ready = true;
			}

			const std::string buttonText{ "Trash the cache with " + graph.graphName };
			if ( ImGui::Button(buttonText.data()) ) {
				if (!graph.calculating) {
					graph.calculating = true;
					graph.ready = false;
				}

				ImGui::Text("Wait for it...");
			}

			if (!graph.ready) {
				allReady = false;
			}
	
		}

		if (window.showCombined) {
			if (allReady) {
				const int steps = 11;
				const float* aData{ graphs[0]->data.durations.data() };
				const float* bData{ graphs[1]->data.durations.data() };

				const float* data_y[] = { graphs[0]->data.durations.data(), graphs[1]->data.durations.data() };
				const unsigned int colors[] = { graphs[0]->color ,graphs[1]->color };
				const float stepsArray[11] = { 1,2,4,8,16,32,64,128,256,512,1024 };

				ImGui::PlotConfig plotConfig;
				plotConfig.values.xs = stepsArray;
				plotConfig.values.ys_list = data_y;

				plotConfig.values.ys_count = 2;
				plotConfig.values.count = steps;

				plotConfig.scale.min = 0.f;
				
				float a = static_cast<float>(*std::max_element(aData, aData + steps));
				float b = static_cast<float>(*std::max_element(bData, bData + steps));

				plotConfig.scale.max = (a > b) ? a : b;

				plotConfig.tooltip.show = true;
				plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";

				plotConfig.frame_size = ImVec2(150, 100);

				plotConfig.grid_y.show = true;
				plotConfig.grid_y.size = plotConfig.scale.max / 10;

				plotConfig.line_thickness = 2.f;
				plotConfig.values.colors = colors;

				ImGui::Plot("CombinedPlot", plotConfig);

				/*const std::string& name{ "CombinedGraph" + window.name };

				ImGui::Plot(name.data(), CreateCombinedPlot(window) );*/
			}
		};

		ImGui::End();
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

