#include "CacheGraphComponent.h"
#include <iostream>

Rinigin::CacheGraphComponent::CacheGraphComponent(GameObject* gameObject) :
	Component(gameObject),
	m_ArraySize{ static_cast<int>(std::pow(2,26)) }
{

	//AddGraph< std::vector<int> >("Int", intContainer);
	//auto returnData = Measure<std::vector<int>>(intContainer, &CacheGraphComponent::Exercise1);
	m_Exercise1Data.graphName = "integers";
	m_Exercise1Data.color = IM_COL32(255, 0, 0, 255);
	m_Exercise1Data.graphType = TypeGraph::intGraph;
	
	m_Exercise2Data.graphName = "GameObjects";
	m_Exercise2Data.color = IM_COL32(0, 255, 0, 255);
	m_Exercise2Data.graphType = TypeGraph::gameObjectGraph;

	m_Exercise3Data.graphName = "GameObjectAlts";
	m_Exercise3Data.color = IM_COL32(0, 0, 255, 255);
	m_Exercise3Data.graphType = TypeGraph::gameObjectAltGraph;

	m_Window1.name = "Exercise 2";
	m_Window1.graphs = { &m_Exercise1Data };

	m_Window2.name = "Exercise 3";
	m_Window2.graphs = { &m_Exercise2Data,&m_Exercise3Data };
	m_Window2.showCombined = true;
}


void Rinigin::CacheGraphComponent::Update() 
{
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame();
	//ImGui::NewFrame(); 

	// Bind the member function to the object instance

	GraphWindow(m_Window1);
	GraphWindow(m_Window2);
}


// Configuration

ImGui::PlotConfig Rinigin::CacheGraphComponent::GetPlotConfiguration()
{
	ImGui::PlotConfig plotConfig;
	plotConfig.tooltip.show = true;
	plotConfig.tooltip.format = "Step: %g\nTime: %.0f ms";
	plotConfig.line_thickness = 2.f;
	plotConfig.frame_size = ImVec2(150, 100);

	return plotConfig;
}

ImGui::PlotConfig Rinigin::CacheGraphComponent::CreateCombinedPlot(GraphWindowData& window)
{
	const float** data_y = &window.data_durations[0];
	unsigned int* colors = &window.data_colors[0];

	ImGui::PlotConfig plotConfig = GetPlotConfiguration();
	plotConfig.values.xs = m_StepsArray;
	plotConfig.values.ys_list = data_y;
	plotConfig.values.ys_count = static_cast<int>(window.graphs.size());

	plotConfig.values.count = m_StepsCount;

	plotConfig.scale.min = 0.f;
	plotConfig.scale.max = window.maxElement;
	plotConfig.values.colors = colors;

	return plotConfig;
}

// Window
void Rinigin::CacheGraphComponent::GraphWindow(GraphWindowData& window)
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

			// Create PlotConfiguration

			const MeasureData& data = graph.data;
			const float* durationArr = data.durations.data();

			ImGui::PlotConfig conf = GetPlotConfiguration();
			conf.values.xs = m_StepsArray;
			conf.values.ys = durationArr;

			conf.values.color = graph.color;
			conf.values.count = m_StepsCount;

			conf.scale.min = 0.f;
			conf.scale.max = *std::max_element(durationArr, durationArr + data.durations.size());

			graph.configuration = conf;

			//

			graph.calculating = false;
			graph.ready = true;

			UpdateWindow(window);
		}

		const std::string buttonText{ "Trash the cache with " + graph.graphName };
		if (ImGui::Button(buttonText.data())) {
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
			ImGui::Text("Combined : ");
			ImGui::Plot("CombinedPlot", window.combinedConfiguration);
		}
	};

	ImGui::End();
}

void Rinigin::CacheGraphComponent::UpdateWindow(GraphWindowData& window)
{
	// Update the data durations storage
	window.data_durations.clear();
	window.data_colors.clear();

	for (const auto& graph : window.graphs) {
		window.data_durations.push_back(graph->data.durations.data());
		window.data_colors.push_back(graph->color);
	}

	window.maxElement = **std::max_element(window.data_durations.data(), window.data_durations.data() + window.data_durations.size());
	window.combinedConfiguration = CreateCombinedPlot(window);
}

