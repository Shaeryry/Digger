#include "CacheGraphComponent.h"
#include <iostream>

dae::CacheGraphComponent::CacheGraphComponent(GameObject* gameObject) :
	Component(gameObject),
	m_MaxSteps{ 1024 },
	m_Step{ 2 },
	m_ArraySize{ static_cast<int>(std::pow(2,26)) }
{
	//const std::vector<int> intContainer(m_ArraySize, 2);

	//AddGraph< std::vector<int> >("Int", intContainer);
	//auto returnData = Measure<std::vector<int>>(intContainer, &CacheGraphComponent::Exercise1);
	m_Exercise1Data.graphName = "Ints";
	m_Exercise1Data.color = IM_COL32(255, 0, 0, 255);
	m_Exercise1Data.graphType = TypeGraph::intGraph;
	
	m_Exercise2Data.graphName = "GameObjects";
	m_Exercise2Data.color = IM_COL32(0, 255, 0, 255);
	m_Exercise2Data.graphType = TypeGraph::gameObjectGraph;

	m_Exercise3Data.graphName = "GameObjectAlts";
	m_Exercise3Data.color = IM_COL32(0, 0, 255, 255);
	m_Exercise3Data.graphType = TypeGraph::gameObjectAltGraph;

	m_Window1.name = "Exercise 1";
	m_Window1.graphs = { &m_Exercise1Data };

	m_Window2.name = "Exercise 2";
	m_Window2.graphs = { &m_Exercise2Data,&m_Exercise3Data };
	m_Window2.showCombined = true;
}


void dae::CacheGraphComponent::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame(); 

	// Bind the member function to the object instance


	GraphWindow(m_Window1);
	GraphWindow(m_Window2);

	/*GraphWindow<gameObject>(m_Exercise2Data, &CacheGraphComponent::Exercise2);
	GraphWindow<gameObjectAlt>(m_Exercise3Data, &CacheGraphComponent::Exercise3);*/

	// Exercise 1

	//ImGui::Begin("Exercise 1");
	//ImGui::InputInt("Samples", &m_Exercise1Data.samples, 0, 0);
	//ImGui::SameLine(); if (ImGui::Button("+")) m_Exercise1Data.samples++;
	//ImGui::SameLine(); if (ImGui::Button("-")) m_Exercise1Data.samples--;
	//
	//if (m_Exercise1Data.ready) {
	//	ImGui::Plot("hi", m_Exercise1Data.configuration);
	//}

	//if (m_Exercise1Data.calculating) {
	//	const std::vector<int> intContainer(m_ArraySize, 2);
	//	m_Exercise1Data.data = Measure<std::vector<int>>(intContainer, &CacheGraphComponent::Exercise1);

	//	// Create PlotConfiguration
	//	const MeasureData& data = m_Exercise1Data.data;
	//	const float* stepsArr = data.steps.data();
	//	const float* durationArr = data.durations.data();

	//	ImGui::PlotConfig conf;
	//	conf.values.xs = stepsArr;
	//	conf.values.ys = durationArr;

	//	conf.values.color = IM_COL32(255, 255, 0, 255);
	//	conf.values.count = static_cast<int>(data.steps.size());

	//	conf.scale.min = 0.f;
	//	conf.scale.max = *std::max_element(durationArr, durationArr + data.durations.size());

	//	conf.tooltip.format = "x=%.0f, y=%.3f";
	//	conf.frame_size = ImVec2(150, 100);
	//	conf.line_thickness = 2.f;

	//	conf.tooltip.show = true;
	//	conf.grid_x.show = true;
	//	conf.grid_y.show = true;

	//	m_Exercise1Data.configuration = conf;
	//	//

	//	m_Exercise1Data.calculating = false;
	//	m_Exercise1Data.ready = true;
	//}

	//if (ImGui::Button("Trash The Cache")) {
	//	if (!m_Exercise1Data.calculating) {
	//		m_Exercise1Data.calculating = true;
	//		m_Exercise1Data.ready = false;
	//	}

	//	ImGui::Text("Wait for it...");
	//}
	//
	//ImGui::End();

}
